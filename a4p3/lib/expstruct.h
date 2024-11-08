/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
 */
/*
 * Modified by Wagner Morais on Aug 2022.
 */
 
#ifndef _EXPSTRUCT_H
#define _EXPSTRUCT_H

struct expStruct {
	int expInt;
	int expFraction;
};

typedef struct expStruct ExpStruct;
/**
 * descripion: The function calculates the approximation of e^i with the taylor series expansion 
 * 
 * pre-condition: The input into iexp() has to be a real value of type integer. 
 *                This conditions does not need to be checked manually, the function call does it by itself
 * 
 * post-condition: The approximate e^i is returned with the integer and the fracional part in an expstruct format
 * 
 */
ExpStruct *iexp (int);

#endif