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
/* Description: Packs n bits into a string of bytes
 
 Arguments: 1. Pointer to bit stream
            2. Pointer to string
            3. Total number of bits
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void pack_to_bytes (bool* data_b, char* packed_string, int num_of_bits)
{
    uint i = 0;
    
    while (num_of_bits > 0)
    {
        *(packed_string + i) = pack_byte(data_b);

        data_b += 8;
        num_of_bits -= 8;
        i++;
    }

    return;
} /* pack_to_bytes */



/*----------------------------------------------------------------*/
/* FUNCTION: pack_byte                                            */
/*----------------------------------------------------------------*/
/* Description: Packs 8 bits into a single byte such that the first
                is placed at the most significant position of the
                byte
 
 Arguments: 1. Pointer to bit stream
 
 Returns:   Packed byte
 
 ------------------------------------------------------------------*/
char pack_byte (bool* data_b)
{
    uint i;
    char packed_byte = 0;
    for (i = 0; i < 8; i++)
    {
        packed_byte <<= 1;
        packed_byte |= *(data_b + i);
        //printf ("%d\n", *(data_b + i));
    }
//    printf("%c\n", packed_byte);
    return packed_byte;
} /* pack_byte */


/*----------------------------------------------------------------*/
/* FUNCTION: unpack_to_bits                                       */
/*----------------------------------------------------------------*/
/* Description: Unpack the word into n bits.

 Arguments: 1. Word to unpack
            2. Pointer to bit stream
            3. n (Number of bits per word

 Returns:   None

 ------------------------------------------------------------------*/
void unpack_to_bits (char* packed_word, bool* data_b, int bits_per_word)
{
    uint i, num_bits;

    while (bits_per_word > 0)
    {
        
        if (bits_per_word >= 8)
        {
            num_bits = 8;
        }
        else
        {
            num_bits = bits_per_word % 8;
        }
        
        unpack_byte (*(packed_word + i), data_b, num_bits);
        
        i++;
        data_b += 8;
        bits_per_word -= 8;
    }

    return;
} /* unpack_to_bits */

/*----------------------------------------------------------------*/
/* FUNCTION: unpack_byte                                          */
/*----------------------------------------------------------------*/
/* Description: Unpack the byte into n bits.

   Arguments:   1. Byte to unpack
                2. Pointer to bit stream
                3. n (Number of bits per word

   Returns:     None
 ------------------------------------------------------------------*/
void unpack_byte (char packed_word, bool* data_b, int num_bits)
{
    uint i;
    for (i = 0; i < num_bits; i++)
    {
        /* MSB is mapped to the first bit out data_b[0] */
        *(data_b + i) = (packed_word >> (num_bits - i - 1)) & 0x01;
        
    }
    return;
} /* unpack_byte */


/*----------------------------------------------------------------*/
/* FUNCTION: float_to_int                                         */
/*----------------------------------------------------------------*/
/* Description: Typecast float stream to int
 
 Arguments:     1. Pointer to int stream
                2. Pointer to float stream
                3. Length of stream
 
 Returns:     None
 ------------------------------------------------------------------*/
void float_to_int (int* int_stream, float* float_stream, int num_of_samples)
{
    int i;
    for (i = 0; i < num_of_samples; i++)
    {
        *(int_stream + i) = (int) *(float_stream + i);
    }
    return;
} /* float_to_int */