//
//  qpsk.c
//  lte_emulator
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qpsk.hpp"
#include "lut.hpp"


/*
 QPSK Constellation Diagram
+-----------------------------------+
|                 | Imag            |
|                 |                 |
|                 |                 |
|    x (0, 1)     |    x (0, 0)     |
|       135       |       45        |
|                 |                 |
|                 |           Real  |
+-----------------------------------+
|                 |                 |
|                 |                 |
|                 |                 |
|    x (1, 0)     |    x (1, 1)     |
|       225       |       315       |
|                 |                 |
|                 |                 |
+-----------------------------------+
+-----------------------------------+

 Generated using http://asciiflow.com
 
*/

class qpsk qpsk_obj;
extern class Lut Lut_obj;

/*----------------------------------------------------------------*/
/* FUNCTION: mod_qpsk_bb_complex                                  */
/*----------------------------------------------------------------*/
/* Description: Generate baseband QPSK symbols in a + jb format.
 
 Arguments: 1. Data bits
            2. Length of data bit stream
            3. Amplitude coefficient
            4. Pointer to QPSK baseband symbol stream (complex)
 
 Returns:   32 bit computed value
 
 ------------------------------------------------------------------*/
void qpsk::mod_qpsk_bb_complex (bool* data_b,
                                uint length_b,
                                float amplitude,
                                GPU_FFT_COMPLEX* qpsk_bb_c)
{
    uint i;
    uint length_s = length_b / QPSK_BITS_PER_SYMBOL;
    for (i = 0; i < length_s; i++)
    {
        /* Even index is mapped to real axis and converted to NRZ */
        (qpsk_bb_c + length_s)->re = 2 * (*(data_b + 2 * length_s)) - 1;
        /* Odd index is mapped to imaginary axis and converted to NRZ */
        (qpsk_bb_c + length_s)->im = 2 * (*(data_b + 2 * length_s + 1)) - 1;
    }
    
    return;
} /* mod_qpsk_bb_complex */

/*----------------------------------------------------------------*/
/* FUNCTION: mod_qpsk_bb_phase                                    */
/*----------------------------------------------------------------*/
/* Description: Generate baseband QPSK symbol phase.
 
 Arguments: 1. Data bits
            2. Length of data bit stream
            3. Amplitude coefficient
            4. Pointer to QPSK baseband symbol stream (phase in degrees)
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void qpsk::mod_qpsk_bb_phase (bool*  data_b,
                              uint   length_b,
                              float  amplitude,
                              float* phase)
{
    uint i;
    uint length_s = length_b / QPSK_BITS_PER_SYMBOL;
    for (i = 0; i < length_s; i++)
    {
        /* If first bit is 1 */
        if ( *(data_b + 2 * i) )
        {
            /* If second bit is 1 */
            if ( *(data_b + 2 * i + 1) )
            {
                *(phase + i) = 315;
            }
            /* If second bit is 0 */
            else
            {
                *(phase + i) = 225;
            }
        }
        /* If first bit is 0 */
        else
        {
            /* If second bit is 1 */
            if ( *(data_b + 2 * i + 1) )
            {
                *(phase + i) = 135;
            }
            /* If second bit is 0 */
            else
            {
                *(phase + i) = 45;
            }
        }

    }
    
    return;
} /* mod_qpsk_bb_phase */


/*----------------------------------------------------------------*/
/* FUNCTION: mod_qpsk_bp_complex                                  */
/*----------------------------------------------------------------*/
/* Description: Generate bandpass QPSK signal using complex
                multiplication of baseband symbol stream and
                complex carrier
 
 Arguments: 1. Data bit stream
            2. Length of data bit stream
            3. Amplitude coefficient
            4. Bandpass signal
            5. Number of cosine/sine carrier cycles per symbol in
               bandpass signal
            6. Sample rate - number of samples per cosine/sine cycle
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void qpsk::mod_qpsk_bp_complex (bool*  data_b,
                                uint   length_b,
                                float  amplitude,
                                float* qpsk_bp_c,
                                float  cycles_per_symbol,
                                uint   sample_rate)
{
    float* neg_sin8_cycle;
    float* cos8_cycle;
    float  intermediate_result[SAMPLES_PER_BASIS_FUNCTION_CYCLE];
    float  amp_s[SAMPLES_PER_BASIS_FUNCTION_CYCLE];
    uint   i;
    uint   length_s = length_b / QPSK_BITS_PER_SYMBOL;

    /* Baseband QPSK modulated signal */
    GPU_FFT_COMPLEX qpsk_bb_c[length_s];
    
    /* Generate baseband QPSK modulated signal */
    mod_qpsk_bb_complex (data_b, length_b, amplitude, &qpsk_bb_c[0]);
    
    /* sample_rate = samples in one consine/sine cycle */
    /* Use only predefined sample rates in lut.h */
    if (sample_rate != SAMPLES_PER_BASIS_FUNCTION_CYCLE)
    {
        printf ("Sample rate %d not defined\n", sample_rate);
        exit (1);
    }
    
    /* Generate basis functions */
    Lut_obj.get_sin8_cycle (&neg_sin8_cycle, 180.0);
    Lut_obj.get_cos8_cycle (&cos8_cycle, 0.0);
    
    for (i = 0; i < SAMPLES_PER_BASIS_FUNCTION_CYCLE; i++)
    {
        amp_s[i] = amplitude;
    }
    
    /* Loop through the number of symbols */
    for (i = 0; i < length_s * SAMPLES_PER_BASIS_FUNCTION_CYCLE; i++)
    {
        /* data.real .* cos (2 * pi * n / 8) */
        intermediate_result[0] = (qpsk_bb_c + 0)->re * *(cos8_cycle + 0);
        intermediate_result[1] = (qpsk_bb_c + 1)->re * *(cos8_cycle + 1);
        intermediate_result[2] = (qpsk_bb_c + 2)->re * *(cos8_cycle + 2);
        intermediate_result[3] = (qpsk_bb_c + 3)->re * *(cos8_cycle + 3);
        intermediate_result[4] = (qpsk_bb_c + 4)->re * *(cos8_cycle + 4);
        intermediate_result[5] = (qpsk_bb_c + 5)->re * *(cos8_cycle + 5);
        intermediate_result[6] = (qpsk_bb_c + 6)->re * *(cos8_cycle + 6);
        intermediate_result[7] = (qpsk_bb_c + 7)->re * *(cos8_cycle + 7);
        
        /* A * data.real .* cos (2 * pi * n / 8) */
        intermediate_result[0] *= amp_s[0];
        intermediate_result[1] *= amp_s[1];
        intermediate_result[2] *= amp_s[2];
        intermediate_result[3] *= amp_s[3];
        intermediate_result[4] *= amp_s[4];
        intermediate_result[5] *= amp_s[5];
        intermediate_result[6] *= amp_s[6];
        intermediate_result[7] *= amp_s[7];

        /* data.im .* -sin (2 * pi * n / 8) */
        qpsk_bp_c[8*i+0] = (qpsk_bb_c + 8*i + 0)->im * *(neg_sin8_cycle + 0);
        qpsk_bp_c[8*i+1] = (qpsk_bb_c + 8*i + 1)->im * *(neg_sin8_cycle + 1);
        qpsk_bp_c[8*i+2] = (qpsk_bb_c + 8*i + 2)->im * *(neg_sin8_cycle + 2);
        qpsk_bp_c[8*i+3] = (qpsk_bb_c + 8*i + 3)->im * *(neg_sin8_cycle + 3);
        qpsk_bp_c[8*i+4] = (qpsk_bb_c + 8*i + 4)->im * *(neg_sin8_cycle + 4);
        qpsk_bp_c[8*i+5] = (qpsk_bb_c + 8*i + 5)->im * *(neg_sin8_cycle + 5);
        qpsk_bp_c[8*i+6] = (qpsk_bb_c + 8*i + 6)->im * *(neg_sin8_cycle + 6);
        qpsk_bp_c[8*i+7] = (qpsk_bb_c + 8*i + 7)->im * *(neg_sin8_cycle + 7);
        
        /* A * data.imag .* -sin (2 * pi * n / 8) */
        qpsk_bp_c[8*i+0] *= amp_s[0];
        qpsk_bp_c[8*i+1] *= amp_s[1];
        qpsk_bp_c[8*i+2] *= amp_s[2];
        qpsk_bp_c[8*i+3] *= amp_s[3];
        qpsk_bp_c[8*i+4] *= amp_s[4];
        qpsk_bp_c[8*i+5] *= amp_s[5];
        qpsk_bp_c[8*i+6] *= amp_s[6];
        qpsk_bp_c[8*i+7] *= amp_s[7];

        /* s(n) = A * data.real .* cos (2 * pi * n / 8) + 
                  A * data.imag .* -sin (2 * pi * n / 8) */
        qpsk_bp_c[8*i+0] += intermediate_result[0];
        qpsk_bp_c[8*i+1] += intermediate_result[1];
        qpsk_bp_c[8*i+2] += intermediate_result[2];
        qpsk_bp_c[8*i+3] += intermediate_result[3];
        qpsk_bp_c[8*i+4] += intermediate_result[4];
        qpsk_bp_c[8*i+5] += intermediate_result[5];
        qpsk_bp_c[8*i+6] += intermediate_result[6];
        qpsk_bp_c[8*i+7] += intermediate_result[7];
    }
    
    
    return;
}


/*----------------------------------------------------------------*/
/* FUNCTION: mod_qpsk_bp_phase                                    */
/*----------------------------------------------------------------*/
/* Description: Generate bandpass QPSK signal by picking out 
                carrier cycles with a preset phase and stiching them
                in a sequence together
 
 Arguments: 1. Data bit stream
            2. Length of data bit stream
            3. Amplitude coefficient
            4. Bandpass signal
            5. Number of cosine/sine carrier cycles per symbol in
               bandpass signal
            6. Sample rate - number of samples per cosine/sine cycle
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void qpsk::mod_qpsk_bp_phase (bool*  data_b,
                              uint   length_b,
                              float  amplitude,
                              float* qpsk_bp_c,
                              float  cycles_per_symbol,
                              uint   sample_rate)
{
    float* cos8_cycle;
    float  amp_s[SAMPLES_PER_BASIS_FUNCTION_CYCLE];
    uint   i, j, cycle_index;
    uint   length_s = length_b / QPSK_BITS_PER_SYMBOL;
    float  phase[NUMBER_OF_SYMBOLS];
    
    for (i = 0; i < NUMBER_OF_DATA_BITS; i++)
    {
        //printf ("%d\n", data_b[i]);
    }
    
    /* Generate baseband QPSK modulated signal */
    mod_qpsk_bb_phase (data_b, length_b, amplitude, phase);
    
    /* sample_rate = samples in one consine/sine cycle */
    /* Use only predefined sample rates in lut.h */
    if (sample_rate != SAMPLES_PER_BASIS_FUNCTION_CYCLE)
    {
        printf ("Sample rate %d not defined\n", sample_rate);
        exit (1);
    }
    
    for (i = 0; i < SAMPLES_PER_BASIS_FUNCTION_CYCLE; i++)
    {
        amp_s[i] = amplitude;
    }
    
    /* Loop through the number of symbols */
    for (i = 0; i < length_s; i++)
    {
        /* Phase shift the cosine carrier */
        Lut_obj.get_cos8_cycle (&cos8_cycle, *(phase + i));

        /* Replicate and scale phase shifted carrier cycles to generate a symbol */
        for (j = 0; j < BASIS_FUNCTION_CYCLES_PER_SYMBOL; j++)
        {
            cycle_index = 8 * (i * 4 + j);
            /* s(n) = A * cos (2 * pi * n / 8 + phi(i)) */
            qpsk_bp_c[cycle_index+0] = cos8_cycle[0] * amp_s[0];
            qpsk_bp_c[cycle_index+1] = cos8_cycle[1] * amp_s[1];
            qpsk_bp_c[cycle_index+2] = cos8_cycle[2] * amp_s[2];
            qpsk_bp_c[cycle_index+3] = cos8_cycle[3] * amp_s[3];
            qpsk_bp_c[cycle_index+4] = cos8_cycle[4] * amp_s[4];
            qpsk_bp_c[cycle_index+5] = cos8_cycle[5] * amp_s[5];
            qpsk_bp_c[cycle_index+6] = cos8_cycle[6] * amp_s[6];
            qpsk_bp_c[cycle_index+7] = cos8_cycle[7] * amp_s[7];
        }
    }
    return;
}

/*----------------------------------------------------------------*/
/* FUNCTION: mod_qpsk                                             */
/*----------------------------------------------------------------*/
/* Description: Generate bandpass QPSK signal with the specified
                data bit stream
 
 Arguments: 1. Data bit stream
            2. Length of data bit stream
            3. Amplitude coefficient
            4. Bandpass signal
            5. Number of cosine/sine carrier cycles per symbol in
               bandpass signal
            6. Sample rate - number of samples per cosine/sine cycle
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void qpsk::mod_qpsk (bool*  data_b,
                     uint   length_b,
                     float  amplitude,
                     float* qpsk_bp_c,
                     float  cycles_per_symbol,
                     uint   sample_rate)
{
    mod_qpsk_bp_phase (data_b,
                       length_b,
                       amplitude,
                       qpsk_bp_c,
                       cycles_per_symbol,
                       sample_rate);
    return;
}

/*----------------------------------------------------------------*/
/* FUNCTION: demod_qpsk                                           */
/*----------------------------------------------------------------*/
/* Description: Coherent QPSK detection and demodulation
 
 Arguments: 1. Sampled bandpass signal
            2. Length of bandpass signal (number of samples)
            3. Sampling rate (Fs/fc)
            4. Pointer to data bits
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void qpsk::demod_qpsk (float* sampled_bp_signal_c,
                       uint size,
                       uint sample_rate,
                       bool* data_b)
{
    int demod_phase;
    float in_phase_energy;
    float quad_phase_energy;
    float* cos_cycle_c;
    float* neg_sin_cycle_c;
    uint cycle_pointer;
    uint data_b_pointer = 0;
    uint number_of_symbols;
    uint i, j;
    
    if (size % CARRIER_SAMPLES_PER_SYMBOL)
    {
        printf ("Length of sampled bandpass signal (%d) is not a multiple of carrier samples per symbol (%d)\n",
                size,
                CARRIER_SAMPLES_PER_SYMBOL);
        exit (1);
    }
    
    /* Number of samples in the sampled bandpass signal */
    number_of_symbols = size / CARRIER_SAMPLES_PER_SYMBOL;

    /* Get a single cycle of cosine at 0 degrees phase offset */
    Lut_obj.get_cos8_cycle (&cos_cycle_c, 0);
    
    /* Get a single cycle of sine at 180 degrees phase offset */
    Lut_obj.get_sin8_cycle (&neg_sin_cycle_c, 180);
    
    /* Loop through the number of symbols */
    for (i = 0; i < number_of_symbols; i++)
    {
        cycle_pointer = i * CARRIER_SAMPLES_PER_SYMBOL;
        
        /* Reset energies after integrating over one symbol */
        in_phase_energy   = 0;
        quad_phase_energy = 0;
        
        /* Loop through each cycle of carrier */
        for (j = 0; j < BASIS_FUNCTION_CYCLES_PER_SYMBOL; j++)
        {
            in_phase_energy   += qpsk_detect_energy (sampled_bp_signal_c + cycle_pointer, &cos_cycle_c[0]);
            quad_phase_energy += qpsk_detect_energy (sampled_bp_signal_c + cycle_pointer, &neg_sin_cycle_c[0]);

            /* Pointer to the current cycle */
            cycle_pointer += 8;
        }
        
        demod_phase = detect_qpsk_phase(in_phase_energy, quad_phase_energy);
        
        /* Map detected phase to data bits */
        map_phase_to_bits (demod_phase, data_b + data_b_pointer);
        data_b_pointer += 2;
    }
    
    for (i = 0; i < NUMBER_OF_DATA_BITS; i++)
    {
        //printf ("%d\n", data_b[i]);
    }

    return;
}


/*----------------------------------------------------------------*/
/* FUNCTION: detect_qpsk_phase                                    */
/*----------------------------------------------------------------*/
/* Description: Detect QPSK symbol phase based on the in-phase and
                quad-phase energies for the current symbol.

 Arguments: 1. In-phase energy
            2. Quad-phase energy
 
 Returns:   Phase in degrees
 
 ------------------------------------------------------------------*/
int qpsk::detect_qpsk_phase (float in_phase_energy, float quad_phase_energy)
{

    int phase = -1;
    
    if (in_phase_energy > positive_energy_threshold)
    {
        /* Symbol in first quadrant */
        if (quad_phase_energy > positive_energy_threshold)
        {
            phase = 45;
        }
        /* Symbol in second quadrant */
        else if (quad_phase_energy < negative_energy_threshold)
        {
            phase = 315;
        }
    }
    else if (in_phase_energy < positive_energy_threshold)
    {
        /* Symbol in third quadrant */
        if (quad_phase_energy > positive_energy_threshold)
        {
            phase = 135;
        }
        /* Symbol in fourth quadrant */
        else if (quad_phase_energy < negative_energy_threshold)
        {
            phase = 225;
        }
    }
    
    if (phase < 0)
    {
        printf ("Symbol not detected. Energies: in-phase %f, quad-phase %f\n", in_phase_energy, quad_phase_energy);
    }
    
    return phase;
    
}

/*----------------------------------------------------------------*/
/* FUNCTION: map_phase_to_bits                                    */
/*----------------------------------------------------------------*/
/* Description: Map QPSK phase to bits data bits
 
 Arguments: 1. Phase
            2. Pointer to data bits
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void qpsk::map_phase_to_bits (uint phase, bool *data_b)
{
    if (phase == 45)
    {
        *data_b = 0;
        *(data_b + 1) = 0;
    }
    else if (phase == 135)
    {
        *data_b = 0;
        *(data_b + 1) = 1;
    }
    else if (phase == 225)
    {
        *data_b = 1;
        *(data_b + 1) = 0;
    }
    else if (phase == 315)
    {
        *data_b = 1;
        *(data_b + 1) = 1;
    }
    else
    {
        printf ("Symbol not detected. Phase = %d\n", phase);
        exit (1);
    }
    
    return;
}


/*----------------------------------------------------------------*/
/* FUNCTION: qpsk_detect_energy                                   */
/*----------------------------------------------------------------*/
/* Description: Detect energy over one carrier cycle for one
                arm (in-phase or quad-phase)
 
 Arguments: 1. Sampled bandpass signal
            2. Locally generated carrier (cos or -sin)
 
 Returns:   Energy accumlated over carrier cycle
 
 ------------------------------------------------------------------*/
float qpsk::qpsk_detect_energy (float* sampled_bp_signal_c, float* local_carrier_cycle_c)
{
    float mixer_product[CARRIER_SAMPLES_PER_SYMBOL];

    memcpy (&mixer_product[0], sampled_bp_signal_c, 8*sizeof (float));
    
    /* Generate in phase product vector */
    mixer_product[0] *= local_carrier_cycle_c[0];
    mixer_product[1] *= local_carrier_cycle_c[1];
    mixer_product[2] *= local_carrier_cycle_c[2];
    mixer_product[3] *= local_carrier_cycle_c[3];
    mixer_product[4] *= local_carrier_cycle_c[4];
    mixer_product[5] *= local_carrier_cycle_c[5];
    mixer_product[6] *= local_carrier_cycle_c[6];
    mixer_product[7] *= local_carrier_cycle_c[7];
    
    
    /* Accumulate product vector */
    mixer_product[0] += mixer_product[4];
    mixer_product[1] += mixer_product[5];
    mixer_product[2] += mixer_product[6];
    mixer_product[3] += mixer_product[7];
    
    mixer_product[0] += mixer_product[2];
    mixer_product[1] += mixer_product[3];
    
    return (mixer_product[0] + mixer_product[1]);
}

/*----------------------------------------------------------------*/
/* FUNCTION: CONSTRUCTOR                                          */
/*----------------------------------------------------------------*/
/* Description: CONSTRUCTOR
 
 Arguments: None
 
 Returns:   None
 
 ------------------------------------------------------------------*/
qpsk::qpsk (void)
{
    
    
    processing_gain = CARRIER_SAMPLES_PER_SYMBOL;
    
    /* Energy of one symbol (ideal) = (Amplitude * processing gain)/(2 * sqrt (2)) */
    positive_energy_threshold = (processing_gain * QPSK_MOD_AMPLITUDE) / 2.8284;
    
    /* Threshold = 0.7 * energy of one symbol */
    positive_energy_threshold *= 0.7;
    
    negative_energy_threshold = -1 * positive_energy_threshold;
}
