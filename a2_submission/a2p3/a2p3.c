/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
*/
/*
 * Modified by Wagner Morais on Aug 2023.
*/
#include <stdio.h>
#include <stdlib.h>
#include "lib/expstruct.h"
#include "lib/led.h"
#include "lib/piface.h"
#include "lib/rpi-systimer.h"

#define LINE 32

int main()
{

	char str[LINE];
	//initialize and clear the piface
	led_init();
	piface_clear();
	piface_puts("DT8025 - A2P3");
	RPI_WaitMicroSeconds(2000000);
    ExpStruct* value;

    int i = 1;
	// cyclic execution
    while (1) {
		value = iexp(i++);
		piface_clear();

		//create a string with the calculated result of the iexp function
		sprintf(str,"%d:%d.%d\n", i-1, value->expInt, value->expFraction);
		piface_puts(str);

		// led_blink();	// or led_blink();

		// restart the calculations once they have gone through all values to 20
		if (i > 20){
			i = 1;
		}

		// free up the memory space taken up by value
		free(value);
    }
	return 0;
}
