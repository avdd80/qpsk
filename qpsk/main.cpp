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
char data = 'A';
char demod_data;
bool data_bits[NUMBER_OF_DATA_BITS];
bool demod_data_bits[NUMBER_OF_DATA_BITS];
extern class qpsk qpsk_obj;

int main (void)
{
    uint i;
    float qpsk_bandpass_c[CARRIER_TOTAL_SAMPLES];
    
    printf ("Message sent = %c\n", data);
    unpack_to_bits ((uint)data, &data_bits[0], 8);
    for (i = 0; i < NUMBER_OF_DATA_BITS; i++)
    {
        printf ("%d\n", data_bits[i]);
    }
    qpsk_obj.mod_qpsk (&data_bits[0], NUMBER_OF_DATA_BITS, QPSK_MOD_AMPLITUDE, &qpsk_bandpass_c[0], BASIS_FUNCTION_CYCLES_PER_SYMBOL, SAMPLES_PER_BASIS_FUNCTION_CYCLE);

    
    qpsk_obj.demod_qpsk(&qpsk_bandpass_c[0], (NUMBER_OF_DATA_BITS/2)*4*8, 8, &demod_data_bits[0]);
    
    demod_data = (char)pack_to_word (&demod_data_bits[0], 8);

    printf ("Demod bits:\n");
    for (i = 0; i < NUMBER_OF_DATA_BITS; i++)
    {
        printf ("%d\n", demod_data_bits[i]);
    }
    printf ("Demodulated message = 0x%X", demod_data);
    
    return 0;
}