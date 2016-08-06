//
//  common.c
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include "common.hpp"
#include <stdio.h>

/*----------------------------------------------------------------*/
/* FUNCTION: log_2                                                */
/*----------------------------------------------------------------*/
/* Description: Returns log to the base 2 of a positive number x,
                x = 2^n, where n is a whole number
 
 Arguments: 1. Positive number x
 
 Returns:   n = log2 (x)
 
 ------------------------------------------------------------------*/
int log_2 (int x)
{
    int log_value = 0;
    /* Since x is expected to be a power of 2, subtracting 1
       sets all the lower bits to 1.
       e.g. 64 - 1 = 63 = 0b111111 */
    x -= 1;
    while (x & 1)
    {
        x >>= 1;
        log_value++;
    }
    return log_value;
} /* log_2 */


/*----------------------------------------------------------------*/
/* FUNCTION: pack_to_word                                         */
/*----------------------------------------------------------------*/
/* Description: Packs n bits into a single word such that the first
                is placed at the most significant position of the 
                n bits
 
 Arguments: 1. Pointer to bit stream
            2. n (number of bits to pack in one word)
 
 Returns:   Packed word
 
 ------------------------------------------------------------------*/
uint pack_to_word (bool* data_b, int bits_per_word)
{
    uint i;
    uint packed_word = 0;
    for (i = 0; i < bits_per_word; i++)
    {
        packed_word |= *(data_b + i);
        packed_word <<= 1;
    }
    printf ("Packed word = 0x%X\n", packed_word);
    return packed_word;
} /* pack_to_word */


/*----------------------------------------------------------------*/
/* FUNCTION: unpack_to_bits                                       */
/*----------------------------------------------------------------*/
/* Description: Unpack the word into n bits.
 
 Arguments: 1. Word to unpack
            2. Pointer to bit stream
            3. n (Number of bits per word
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void unpack_to_bits (uint packed_word, bool* data_b, int bits_per_word)
{
    uint i;
    printf ("Unpacking 0x%X:\n", packed_word);
    for (i = 0; i < bits_per_word; i++)
    {
        /* MSB is mapped to the first bit out data_b[0] */
        *(data_b + i) = (packed_word >> (bits_per_word - i)) & 0x01;
        printf ("%d at %d\n", *(data_b + i), (bits_per_word - i));
        
    }
    printf ("End\n");
} /* unpack_to_bits */