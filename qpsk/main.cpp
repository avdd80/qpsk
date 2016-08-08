//
//  main.c
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include <stdio.h>
#include "types.hpp"
#include "qpsk.hpp"
#include "common.hpp"

//bool data_bits[NUMBER_OF_DATA_BITS] = {0, 0, 0, 1, 1, 1, 0, 0};
char data[4] = {'A', 'B', 'C', 'D'};
char demod_data[4];
bool data_bits[NUMBER_OF_DATA_BITS];
bool demod_data_bits[NUMBER_OF_DATA_BITS];
extern class qpsk qpsk_obj;

int main (void)
{
    float qpsk_bandpass_c[CARRIER_TOTAL_SAMPLES];
    
    printf ("Message sent = %s\n", data);
    unpack_to_bits ((uint*)&data[0], &data_bits[0], NUMBER_OF_DATA_BITS);

    qpsk_obj.mod_qpsk (&data_bits[0], NUMBER_OF_DATA_BITS, QPSK_MOD_AMPLITUDE, &qpsk_bandpass_c[0], BASIS_FUNCTION_CYCLES_PER_SYMBOL, SAMPLES_PER_BASIS_FUNCTION_CYCLE);

    
    qpsk_obj.demod_qpsk(&qpsk_bandpass_c[0], (NUMBER_OF_DATA_BITS/QPSK_BITS_PER_SYMBOL)*4*8, 8, &demod_data_bits[0]);
    
    pack_to_word (&demod_data_bits[0], (uint*)&demod_data[0], NUMBER_OF_DATA_BITS);

    printf ("Demodulated message = %s\n", demod_data);
    
    return 0;
}