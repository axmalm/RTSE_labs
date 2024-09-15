/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
	Wagner de Morais (Wagner.deMorais@hh.se)
*/

#include <stdio.h>
#include "uart.h"
#include "iregister.h"

#define LINE 80

void displayMenu() {
	printf("\n--- Menu ---\n");
    printf("1. Set All Bits to 1\n");
    printf("2. Clear All Bits (Set to 0)\n");
    printf("3. Shift Left\n");
    printf("4. Shift Right\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
}

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

	uart_puts("DT8025 - Assignment 1");
	// Enter name 
	uart_puts("Enter your name: ");

	int i = 0; 
	// multiple times until receiving a new line.
	

	uarts_puts("Welcome ");
	uart_puts(str);
	str[] = 
	uart_puts("Enter a integer number (32 bit): ");
	inumber = uart_getc();
	
	
	// atoi(str) will result a number.
}