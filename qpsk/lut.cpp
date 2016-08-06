//
//  lut.c
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#include "lut.hpp"
#include "types.hpp"
#include <math.h>

class Lut Lut_obj;


/*----------------------------------------------------------------*/
/* FUNCTION: get_cos8_cycle                                       */
/*----------------------------------------------------------------*/
/* Description: Generate sampled cosine basis function:
 
 s(n) = cos ((2 * pi * n / 8) + phi)
 
 Arguments: 1. Pointer to pointer to cosine samples (8 samples 
                cycle) at a preset phase
            2. Phase of the cosine basis function
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void Lut::get_cos8_cycle (float** cos_cycle, float phi)
{
    int cos_index = (int) round (phi / 45.0);

    /* Make the angle positive */
    while (cos_index < 0)
    {
        cos_index += 8;
    }

    cos_index %= 8;
    
    *cos_cycle = &cos8[0] + cos_index;
    
    return;
}

/*----------------------------------------------------------------*/
/* FUNCTION: get_sin8_cycle                                       */
/*----------------------------------------------------------------*/
/* Description: Generate sampled sine basis function:
 
 s(n) = sin ((2 * pi * n / 8) + phi + 3*pi/4)
 
 Arguments: 1. Pointer to pointer to sine samples (8 samples
                cycle) at a preset phase
            2. Phase of the sine basis function
 
 Returns:   None
 
 ------------------------------------------------------------------*/
void Lut::get_sin8_cycle (float** sin_cycle, float phi)
{
    get_cos8_cycle (sin_cycle, phi + 270.0);
    return;
}