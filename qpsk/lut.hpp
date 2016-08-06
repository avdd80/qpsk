//
//  lut.h
//  qpsk
//
//  Created by Abhijeet Deshpande on 7/31/16.
//  Copyright © 2016 Abhijeet Deshpande. All rights reserved.
//

#ifndef lut_h
#define lut_h

class Lut
{
private:
    /* 2 cycles of cos (2 * pi * n/8) */
    float cos8[16] =
    {
        1,
        0.70710678118,
        0,
        -0.70710678118,
        -1,
        -0.70710678118,
        0,
        0.70710678118,
        1,
        0.70710678118,
        0,
        -0.70710678118,
        -1,
        -0.70710678118,
        0,
        0.70710678118,
    };
    
public:
    /*----------------------------------------------------------------*/
    /* FUNCTION: get_cos8_cycle                                       */
    /*----------------------------------------------------------------*/
    void get_cos8_cycle (float** cos_cycle, float phi);


    /*----------------------------------------------------------------*/
    /* FUNCTION: get_sin8_cycle                                       */
    /*----------------------------------------------------------------*/
    void get_sin8_cycle (float** sin_cycle, float phi);
};

#endif /* lut_h */
