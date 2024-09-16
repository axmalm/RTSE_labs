/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
	Wagner de Morais (Wagner.deMorais@hh.se)
*/

#include <stdio.h>
//#include "uart.h"
#include "iregister.h"

#define LINE 80

int main()
{
	iRegister r;
	char str[LINE];
	char c;
	int inumber, inibble, ibit, ishift = 0;

	// Using the uart
	// First, initialize and clear the channel
	uart_init();
	uart_clear();

	uart_puts("DT8025 - Assignment 1\n");
	// Enter name
	uart_puts("Enter your name: ");

	//scanf("%s",str);
	int i = 0;
	while(c = uart_getc() != "\n"){
        str[i] = c;
        i++;
	}
	uart_puts("\nWelcome %s!", str);

	uart_puts("\nEnter an integer number (32 bit): ");
    int i = 0;
	while(c = uart_getc() != "\n"){
        str[i] = c;
        i++;
	}
	inumber = atoi(str);
	//scanf("%d", &inumber);

    r.content = (inumber - '0');

	uart_puts("Enter the bit position (0<=bit<=31): ");
	int i = 0;
	while(c = uart_getc() != "\n"){
        str[i] = c;
        i++;
	}
	ibit = atoi(str);
	//scanf("%d", &ibit);

	uart_puts("Enter the nibble position (0<=bit<=7): ");
	int i = 0;
    while(c = uart_getc() != "\n"){
        str[i] = c;
        i++;
	}
	inibble = atoi(str);
	//scanf("%d", &inibble);

	uart_puts("Enter the number of bits to shift (1<=bit<=31): ");
	int i = 0;
    while(c = uart_getc() != "\n"){
        str[i] = c;
        i++;
	}
	ishift = atoi(str);
	//scanf("%d", &ishift);

	uart_puts("You entered number %i", inumber);


    setAll(&r);
    uart_puts("\n\nsetAll(&r) returned %i", r.content);

    r.content = inumber;
    resetAll(&r);
    uart_puts("\nresetAll(&r) returned %i", r.content);
    uart_puts("%c",reg2str(r));

    r.content = inumber;
    setBit(ibit, &r);
    uart_puts("\nsetBit(%i, &r) returned %i", ibit, r.content);

    r.content = inumber;
    int A = getBit(ibit, &r);
    uart_puts("\ngetBit(%i, &r) returned %i", ibit, A);

    r.content = inumber;
    resetBit(ibit, &r);
    uart_puts("\nresetBit(%i, &r) returned %i", ibit, r.content);

    r.content = inumber;
    assignNibble(inibble, 1 , &r);
    uart_puts("\nassignNibble(%i, 1, &r) returned %i", inibble, r.content);

    r.content = inumber;
    int B = getNibble(inibble, &r);
    uart_puts("\ngetNibble(%i, &r) returned %i", inibble, B);

    r.content = inumber;
    shiftRight(ishift, &r);
    uart_puts("\nshiftRight(%i, &r) returned %i", ishift, r.content);

    r.content = inumber;
    shiftLeft(ishift, &r);
    uart_puts("\nshiftLeft(%i, &r) returned %i", ishift, r.content);

	// However, to get a number, you need to call uart_getc
	// multiple times until receiving a new line.
	// The results of each call to uart_getc can be stored into str
	// atoi(str) will result a number.
}

