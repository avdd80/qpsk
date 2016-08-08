#ifndef common_h
#define common_h

//
//  common.c
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include "common.hpp"
#include "types.hpp"

/*----------------------------------------------------------------*/
/* FUNCTION: log_2                                                */
/*----------------------------------------------------------------*/
int log_2 (int x);


/*----------------------------------------------------------------*/
/* FUNCTION: antilog_2                                            */
/*----------------------------------------------------------------*/
/* Description: Returns antilog of number n to the power 2
 
 Arguments: 1. Positive whole number n
 
 Returns:   x = 2^n
 
 ------------------------------------------------------------------*/
#define antilog_2(n) (1<<n)



/*----------------------------------------------------------------*/
/* FUNCTION: pack_to_word                                         */
/*----------------------------------------------------------------*/
void pack_to_bytes (bool* data_b, char* packed_string, int num_of_bits);

/*----------------------------------------------------------------*/
/* FUNCTION: pack_byte                                            */
/*----------------------------------------------------------------*/
char pack_byte (bool* data_b);

/*----------------------------------------------------------------*/
/* FUNCTION: unpack_to_bits                                       */
/*----------------------------------------------------------------*/
void unpack_to_bits (char* packed_word, bool* data_b, int bits_per_word);

/*----------------------------------------------------------------*/
/* FUNCTION: unpack_byte                                          */
/*----------------------------------------------------------------*/
void unpack_byte (char packed_word, bool* data_b, int num_bits);

/*----------------------------------------------------------------*/
/* FUNCTION: float_to_int                                         */
/*----------------------------------------------------------------*/
void float_to_int (int* int_stream, float* float_stream, int num_of_samples);

#endif /* common_h */
