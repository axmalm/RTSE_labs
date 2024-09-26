/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
 */
/*
 * Modified by Wagner Morais on Aug 2022.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include "expstruct.h"
#include "rpi-systimer.h"

// calculates the factorial value of i
double factorial(int i){
    if (i <= 1){
        return 1.0;
    }
    // create a double to store the result, in the for loop multiply it with j (the number of iterations of the loop)
    double res = 1.0;
    for (int j = 2; j <= i; j++){
        res *= j;
    }
    return res;
}

// function to calculate x^i
double power(int x, int i){
    double res = 1;
    for (int j = 1; j < i+1; j++){
        res *= x;
    }
    return res;
}

// taylor series expansion of e^x
ExpStruct *iexp(int x){
    int n = 50;
    double res = 1;
    double diff = 1;
    double prev_res = 0;

    // create a for loop that calculates the sum of the taylor series expansion
    // for (int i = 1; (i < n) || (diff > 0.01); i++) {
    for (int i = 1; i < n; i++){
        prev_res = res;
        res += (power(x, i) / factorial(i));

        // calculate the difference in the current solution vs the previous one, and only go around the loop until certain complexity is reached
        if (i==1){
            diff = res;
        } else{
            diff = res - prev_res;
        }
        // diff = (i == 1) ? res : res - diff;

        // delay the result
        RPI_WaitMicroSeconds(50000);

        if (diff < 0.01){
            break;
        }
    }

	ExpStruct *e = malloc(sizeof(ExpStruct));

    if (e == NULL) {
        return NULL;
    }

    // store the solution in e as separate integer and fractional values
    e->expInt = (int)res;
    e->expFraction = (int)((res - e->expInt) * 100);
	return e;
}
