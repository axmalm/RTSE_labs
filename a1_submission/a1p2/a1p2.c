/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Wagner de Morais (Wagner.deMorais@hh.se)
*/

#include "led.h"

int main()
{
	led_init();
    // repeatedly calls led_blink()
	while (1) {
        led_blink();
    }
}