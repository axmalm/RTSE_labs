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
#include "led.h"

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
    int n = 50;
    double res = 1;
    double diff = 1;
    double prev_res = 0;
    // for (int i = 1; (i < n) || (diff > 0.01); i++) {
    int i;
    // calcuting the sum of values
    for (i = 1; i < n; i++){
        prev_res = res;
        res += (power(x, i) / factorial(i));
        if (i==1){
            diff = res;
        } else{
            diff = res - prev_res;
        }
        // diff = (i == 1) ? res : res - diff;
        RPI_WaitMicroSeconds(50000);
        //toggle the led when the code went around the loop 3 times
        if (i % 3 == 0) {
            led_toggle();
        }
        if (diff < 0.01 && i >= 3 && i % 3 == 0){
        // if (diff < 0.01){
            break;
        }
    }
	ExpStruct *e = malloc(sizeof(ExpStruct));
    if (e == NULL) {
        return NULL;
    }
    // set the integer and fractional part in e
    e->expInt = (int)res;
    e->expFraction = (int)((res - e->expInt) * 100);
    // e->expFraction = i;
	return e;
}