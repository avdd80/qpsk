//
//  gray_code.c
//  qpsk
//
//  Created by Abhijeet Deshpande on 8/1/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include "gray_code.hpp"
#include "common.hpp"

/* 6 bit gray code LUT */
uint gray_code_lut[64] = {
    0,
    1,
    3,
    2,
    7,
    6,
    4,
    5,
    8,
    9,
    11,
    10,
    15,
    14,
    12,
    13,
    24,
    25,
    27,
    26,
    31,
    30,
    28,
    29,
    16,
    17,
    19,
    18,
    23,
    22,
    20,
    21,
    56,
    57,
    59,
    58,
    63,
    62,
    60,
    61,
    48,
    49,
    51,
    50,
    55,
    54,
    52,
    53,
    32,
    33,
    35,
    34,
    39,
    38,
    36,
    37,
    40,
    41,
    43,
    42,
    47,
    46,
    44,
    45
};

void map_gray (bool* data_b, bool* gray_data_b, uint length_b, uint modulation_order)
{
    uint i;
    char packed_word;
    uint mod_order_log;
    uint length_s = length_b/modulation_order;
    
    mod_order_log = log_2 (modulation_order);
    
    for (i = 0; i < length_s; i++)
    {
        /* Pack bits to word */
        pack_to_bytes (data_b + length_b * mod_order_log, &packed_word, mod_order_log);
        
        /* Pick out gray code from the LUT */
        packed_word = gray_code_lut[packed_word];
        
        /* Unpack gray coded word to bits */
        unpack_to_bits ((char*)&packed_word, gray_data_b + i * mod_order_log, mod_order_log);
    }
    
    return;
}