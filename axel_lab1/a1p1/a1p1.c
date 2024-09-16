/*
    Part of the Real-Time Embedded Systems course at Halmstad University
    Copyright (c) 2017, Sebastian Kunze <sebastian.kunze@hh.se>
    All rights reserved.
	Wagner de Morais (Wagner.deMorais@hh.se)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "uart.h"
#include "lib/iregister.h"

#define LINE 80

void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Set bit to 1\n");
    printf("2. Set all bits to 1\n");
    printf("3. Get bit\n");
    printf("4. Assign nibble\n");
    printf("5. Get nibble\n");
    printf("6. reg2str\n");
    printf("7. Shift right\n");
    printf("8. Shift left\n");
    printf("9. Reset bit\n");
    printf("Choose an option: ");
}

int main() {
    iRegister r;  // Initialize the register
    r.content = 0;  // Set the content to zero
    char str[LINE];
    int inumber, inibble, ibit, ishift;
    int choice;
    char name[LINE];

    // Initialize and clear the UART channel
    // uart_init();
    // uart_clear();

    printf("DT8025 - Assignment 1\n");
    // Enter name
    printf("Enter your name: ");
    scanf("%s", name);

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                // Set bit to 1
                printf("Enter bit number (0-31): ");
                scanf("%d", &ibit);
                setBit(ibit, &r);
                printf("Bit %d set to 1.\n", ibit);
                break;
            case 2:
                // Set all bits to 1
                setAll(&r);
                printf("All bits set to 1.\n");
                break;
            case 3:
                // Get bit
                printf("Enter bit number (0-31): ");
                scanf("%d", &ibit);
                inumber = getBit(ibit, &r);
                printf("Bit %d is %d.\n", ibit, inumber);
                break;
            case 4:
                // Assign nibble
                printf("Enter value for 'a' (0-15): ");
                int a;
                scanf("%d", &a);
                printf("Enter value for 'b' (0-15): ");
                int b;
                scanf("%d", &b);
                assignNibble(a, b, &r);
                printf("Assigned nibble with a = %d, b = %d.\n", a, b);
                break;
            case 5:
                // Get nibble
                printf("Enter nibble index (1-8): ");
                scanf("%d", &inibble);
                inumber = getNibble(inibble, &r);
                printf("Nibble %d is %d.\n", inibble, inumber);
                break;
            case 6:
                // Convert register to string
                {
                    char *regStr = reg2str(r);
                    if (regStr != NULL) {
                        printf("Register: %s\n", regStr);
                        free(regStr);  // Free the allocated memory
                    } else {
                        printf("Error converting register to string.\n");
                    }
                }
                break;
            case 7:
                // Shift right
                printf("Enter number of positions to shift right: ");
                scanf("%d", &ishift);
                shiftRight(ishift, &r);
                printf("Register shifted right by %d positions.\n", ishift);
                break;
            case 8:
                // Shift left
                printf("Enter number of positions to shift left: ");
                scanf("%d", &ishift);
                shiftLeft(ishift, &r);
                printf("Register shifted left by %d positions.\n", ishift);
                break;
            case 9:
                // Reset bit
                printf("Enter bit number (0-31): ");
                scanf("%d", &ibit);
                resetBit(ibit, &r);
                printf("Bit %d reset to 0.\n", ibit);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    return 0;
}