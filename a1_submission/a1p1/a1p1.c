/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
	Wagner de Morais (Wagner.deMorais@hh.se)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/uart.h"
#include "lib/iregister.h"

#define LINE 80

// create a function to get typed in characters from the uart
void inputSerial(char *str){
    char c;
    int i = 0;
	while(i < LINE - 1){
        c = uart_getc();
        if (c == ' ' || c == '\n' || c == '\r'){
            break;
        }
        str[i++] = c;
	}
    str[i] = '\0';
    uart_puts(str);
}

int main()
{
	iRegister r;
	char str[LINE];
	int inumber, inibble, ibit, ishift = 0;

	// Using the uart
	// First, initialize and clear the channel
	uart_init();
	uart_clear();

	uart_puts("DT8025 - Assignment 1\n");
	// Enter name
	uart_puts("Enter your name: ");
    // get the name from the user
	inputSerial(str);


	uart_puts("\nWelcome ");
	uart_puts(str);

	uart_puts("\nEnter an integer number (32 bit): ");
	inputSerial(str);

    // saves inumber as a integer while storing the string value in printinumber to be able to print it out
    char printinumber[LINE];
    strcpy(printinumber, str);
	inumber = atoi(str);

    // repeat the process for ibit (l.55)
	uart_puts("\nEnter the bit position (0<=bit<=31): ");
    inputSerial(str);
    char printibit[LINE];
    strcpy(printibit, str);
	ibit = atoi(str);

	// repeat the process for inibble (l.55)
	uart_puts("\nEnter the nibble position (0<=bit<=7): ");
    inputSerial(str);
    char printinibble[LINE];
    strcpy(printinibble, str);
	inibble = atoi(str);

	//repeat the process for ishift (l.55)
	uart_puts("\nEnter the number of bits to shift (1<=bit<=31): ");
    inputSerial(str);
    char printishift[LINE];
    strcpy(printishift, str);
	ishift = atoi(str);

	uart_puts("\nYou entered number ");
	uart_puts(printinumber);

    // use the previously created functions and print solutions to the uart
    // the print2uart function is used to create a string, which is then printed to the uart using
    // uart_puts internally
    setAll(&r);
    print2uart("\n\nsetAll(&r) returned %d", r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    resetAll(&r);
    print2uart("\nresetAll(&r) returned %d", r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    setBit(ibit, &r);
    print2uart("\nsetBit ( %d, &r) returned %d", ibit, r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    int A = getBit(ibit, &r);
    print2uart("\ngetBit ( %d, &r) returned %d", ibit, A);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    resetBit(ibit, &r);
    print2uart("\nresetBit ( %d, &r) returned %d", ibit, r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    assignNibble(inibble, 1 , &r);
    print2uart("\nassignNibble ( %d, 1, &r) returned %d", inibble, r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    int B = getNibble(inibble, &r);
    print2uart("\ngetNibble( %d, &r) returned %d", inibble, B);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    shiftRight(ishift, &r);
    print2uart("\nshiftRight( %d, &r) returned %d", ishift, r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

    r.content = inumber;
    shiftLeft(ishift, &r);
    print2uart("\nshiftLeft( %d, &r) returned %d", ishift, r.content);
    uart_puts("   (");
    uart_puts(reg2str(r));
    uart_puts(")");

	// However, to get a number, you need to call uart_getc
	// multiple times until receiving a new line.
	// The results of each call to uart_getc can be stored into str
	// atoi(str) will result a number.
    return 0;
}

