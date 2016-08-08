//
//  qpsk.h
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#ifndef qpsk_h
#define qpsk_h

#include "types.hpp"
#include "qpsk.hpp"

#define NUMBER_OF_CHAR_DATA             32

/* Number of data bits in the data stream */
#define NUMBER_OF_DATA_BITS              NUMBER_OF_CHAR_DATA*8

/* Amplitude of transmitted signal */
#define QPSK_MOD_AMPLITUDE               200

/* 2 bits map to 1 QPSK symbol */
#define QPSK_BITS_PER_SYMBOL             2

/* 4 cosine/sine cycles per symbol */
#define BASIS_FUNCTION_CYCLES_PER_SYMBOL 4

/* 8 samples per cosine/sine cycle */
#define SAMPLES_PER_BASIS_FUNCTION_CYCLE 8

/* Samples per symbol =                 Number of cycles per symbol *
                                        Samples per symbol */
#define CARRIER_SAMPLES_PER_SYMBOL      (BASIS_FUNCTION_CYCLES_PER_SYMBOL *  \
                                        SAMPLES_PER_BASIS_FUNCTION_CYCLE)

/* Number of bits per symbol */
#define BITS_PER_SYMBOL                 QPSK_BITS_PER_SYMBOL

/* Total number of symbols = Total data bits / bits per symbol */
#define NUMBER_OF_SYMBOLS               NUMBER_OF_DATA_BITS/BITS_PER_SYMBOL

/* Total number of cycles = Total number of symbols * Number of cycles per symbol */
#define NUMBER_TOTAL_CYCLES             (NUMBER_OF_SYMBOLS *                 \
                                        BASIS_FUNCTION_CYCLES_PER_SYMBOL)

/* Total number of carrier samples =    Number of symbols *
                                        Number of cycles per symbol *
                                        Samples per symbol */
#define CARRIER_TOTAL_SAMPLES           (NUMBER_OF_SYMBOLS *                 \
                                        CARRIER_SAMPLES_PER_SYMBOL)

class qpsk
{
    
private:
    
    /* Thresholds to detect a symbol in in- and quad phase arms over
       one carrier cycle */
    float positive_energy_threshold;
    float negative_energy_threshold;
    float processing_gain;
    
    /*----------------------------------------------------------------*/
    /* FUNCTION: mod_qpsk_bb_complex                                  */
    /*----------------------------------------------------------------*/
    void mod_qpsk_bb_complex (bool* data_b,
                              uint  length_b,
                              float amplitude,
                              GPU_FFT_COMPLEX* qpsk_bb_c);
    
    
    /*----------------------------------------------------------------*/
    /* FUNCTION: mod_qpsk_bb_phase                                    */
    /*----------------------------------------------------------------*/
    void mod_qpsk_bb_phase (bool*  data_b,
                            uint   length_b,
                            float  amplitude,
                            float* phase);
    
    
    /*----------------------------------------------------------------*/
    /* FUNCTION: mod_qpsk_bp_complex                                  */
    /*----------------------------------------------------------------*/
    void mod_qpsk_bp_complex (bool*  data_b,
                              uint   length_b,
                              float  amplitude,
                              float* qpsk_bp_c,
                              float  cycles_per_symbol,
                              uint   sample_rate);
    
    
    /*----------------------------------------------------------------*/
    /* FUNCTION: mod_qpsk_bp_phase                                    */
    /*----------------------------------------------------------------*/
    void mod_qpsk_bp_phase (bool*   data_b,
                            uint    length_b,
                            float   amplitude,
                            float*  qpsk_bp_c,
                            float   cycles_per_symbol,
                            uint    sample_rate);

    /*----------------------------------------------------------------*/
    /* FUNCTION: qpsk_detect_energy                                   */
    /*----------------------------------------------------------------*/
    float qpsk_detect_energy (float* sampled_bp_signal_c, float* local_carrier_cycle_c);
    
    /*----------------------------------------------------------------*/
    /* FUNCTION: detect_qpsk_phase                                    */
    /*----------------------------------------------------------------*/
    int detect_qpsk_phase (float in_phase_energy, float quad_phase_energy);
    
    /*----------------------------------------------------------------*/
    /* FUNCTION: map_phase_to_bits                                    */
    /*----------------------------------------------------------------*/
    void map_phase_to_bits (uint phase, bool *data_b);
    
public:

    /*----------------------------------------------------------------*/
    /* FUNCTION: mod_qpsk                                             */
    /*----------------------------------------------------------------*/
    void mod_qpsk (bool*  data_b,
                   uint   length_b,
                   float  amplitude,
                   float* qpsk_bp_c,
                   float  cycles_per_symbol,
                   uint   sample_rate);

    /*----------------------------------------------------------------*/
    /* FUNCTION: demod_qpsk                                           */
    /*----------------------------------------------------------------*/
    void demod_qpsk (float* sampled_bp_signal_c,
                     uint size,
                     uint sample_rate,
                     bool* data_b);


    /*----------------------------------------------------------------*/
    /* FUNCTION: CONSTRUCTOR                                          */
    /*----------------------------------------------------------------*/
    qpsk (void);

};
#endif /* qpsk_h */
