//
//  main.c
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "types.hpp"
#include "qpsk.hpp"
#include "common.hpp"

//bool data_bits[NUMBER_OF_DATA_BITS] = {0, 0, 0, 1, 1, 1, 0, 0};
char data[NUMBER_OF_CHAR_DATA];
char demod_data[NUMBER_OF_CHAR_DATA];
bool data_bits[NUMBER_OF_DATA_BITS];
bool demod_data_bits[NUMBER_OF_DATA_BITS];
extern class qpsk qpsk_obj;

int main (void)
{
    float qpsk_bandpass_c[CARRIER_TOTAL_SAMPLES];
    int qpsk_bandpass_int_c[CARRIER_TOTAL_SAMPLES];
    
    sprintf(data, "%x This is a test message", 0xCCAA);
    /* Bit sequence 1111 0000 11 00 1 0 1 0 */
    data[0] = 0xF;
    data[1] = 0x0;
    data[2] = 0xC;
    data[3] = 0xA;
    
    printf ("Message sent = %s\n", data);
    unpack_to_bits (&data[0], &data_bits[0], NUMBER_OF_DATA_BITS);

    qpsk_obj.mod_qpsk (&data_bits[0], NUMBER_OF_DATA_BITS, QPSK_MOD_AMPLITUDE, &qpsk_bandpass_c[0], BASIS_FUNCTION_CYCLES_PER_SYMBOL, SAMPLES_PER_BASIS_FUNCTION_CYCLE);

    float_to_int (&qpsk_bandpass_int_c[0], &qpsk_bandpass_c[0], CARRIER_TOTAL_SAMPLES);
    
    qpsk_obj.demod_qpsk(&qpsk_bandpass_c[0], (NUMBER_OF_DATA_BITS/QPSK_BITS_PER_SYMBOL)*4*8, 8, &demod_data_bits[0]);

    
    pack_to_bytes (&demod_data_bits[0], &demod_data[0], NUMBER_OF_DATA_BITS);

    printf ("Demodulated message = %s\n", demod_data);
    
    return 0;
}