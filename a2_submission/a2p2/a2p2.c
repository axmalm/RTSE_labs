/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
	Wagner de Morais (Wagner.deMorais@hh.se)
*/

#include <stdio.h>
#include <stdlib.h>
#include "expstruct.h"
#include "piface.h"
#include "rpi-systimer.h"

#define LINE 32

int main()
{
	// initialize and clear the piface
	piface_init();
	piface_clear();
	
	piface_puts("DT8025 - A2P2");
	RPI_WaitMicroSeconds(2000000);	
    piface_clear();

    ExpStruct* value;
    char buffer[LINE];
	// Loops through the values 1-20 and outputs the solution to the e^i function
	for(int i = 1; i <= 20; i++){
		value = iexp(i);
		sprintf(buffer,"e^%d = %d.%02d", i, value->expInt, value->expFraction);
		piface_puts(buffer);
		RPI_WaitMicroSeconds(2000000);	
		piface_clear();		
	}	
	// freeing up the memory used up by value
	free(value);
	return 0;

}