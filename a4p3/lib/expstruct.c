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

// calculates the facrotial value of i
double factorial(int i){
    if (i <= 1){
        return 1.0;
    }
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
    int n = 20;
    double res = 1;
    for (int i = 1; i < n; i++) {
        res += (power(x, i) / factorial(i));
    }
	ExpStruct *e = malloc(sizeof(ExpStruct));
    if (e == NULL) {
        return NULL;
    }
    e->expInt = (int)res;
    e->expFraction = (int)((res - e->expInt) * 100);
	return e;
}