//  Created by Mohammadreza Mousavi [mohmou] on 9/5/14.
//  Updated by Masoumeh Taromirad on 11/08/16.
//  Updated by Wagner Morais and Johannes van Esch on 28/08/18.
//  Updated by Wagner Morais and Hazem Ali on 26/08/21.
//  Copyright (c) 2014 by Mohammadreza Mousavi [mohmou]. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include "iregister.h"

void resetAll(iRegister *A)
{
	// Pre-condition
	if(A == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to resetAll\n");
		return;
	}

    //sets all bits in the content to zeros
	A->content = 0;

	//Post-condition
    if((A->content) != 0)
	{
		fprintf(stderr, "Error: Failed to reset the Register\n");
		return;
	}
}


void setBit(int i, iRegister *R)
{
    // Pre-condition
    if( i < 0 || i > 31)
	{
		fprintf(stderr,"Error: Invalid bit\n");
		return;
	}
	if(R == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to setBit\n");
		return;
	}

    // creates a mask with the i'th bit as a one and bitwise ORs this with the content in the Register
    R->content |= (1 << i);

    // Post-condition
    if((R->content & (1 << i)) == 0)
	{
		fprintf(stderr, "Error: Failed to set Bit\n");
		return;
	}
}

void setAll(iRegister *r)
{
	// pre condition
	if (r == NULL){
		fprintf(stderr, " Error: A NULL pointer was given to setAll\n");
		return;
	}

	r->content = ~0;

	// post-condition
	if((r->content != -1)){
		fprintf(stderr, "Error: Failed to set all bits to 1\n");
		return;
	}
}


int getBit(int i, iRegister *R)
{
    // Pre-condition
    if( i < 0 || i > 31)
	{
		fprintf(stderr,"Error: Invalid bit\n");
		return;
	}
	if(R == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to getBit\n");
		return;
	}

	//creates an integer to store the value of the content before the function is exectuted.
	int beg = R->content;
	//creates the return integer A, assuming it is a one
    int A = 1;

    //creates a copy of the content, so the content itself doesn't get changed
    int con = R->content;
    // creates a mask with only the i'th value being high - this is bitwise ANDed with the content. if the value is zero, the overall output changes to zero
	if((con &= (1 << i))== 0){
        A = 0;
    }
    //returns the value of the i'th bit
    return A;


	// Post-conditions
    if((R->content & (1<<i)) != A)
	{
		fprintf(stderr, "Error: Failed to get Bit\n");
		return;
	}
    if(R->content != beg)
	{
		fprintf(stderr, "Error: Failed to get Bit\n");
		return;
	}
}

void assignNibble(int a, int b, iRegister *r){

	// pre-conditions
	if (r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to assignNibble\n");
		return;
	}
	if (a < 0 || a > 7 || b < 0 || b > 15){
		fprintf(stderr, "Error: Invalid a or b\n");
		return;
	}

	int content_before = r->content;

	//clear out the selected nibble
	int mask = ~(00001111 << ((a)*4));
	r->content &= mask;

	r->content += (b << (a*4));

	// post-condition
	// Check that the other bits in the content have not been altered
	if ((content_before & mask) != (r->content & mask)){
		fprintf(stderr, "Error: Other bits have been altered");
		return;
	}
}

int getNibble(int i, iRegister *R)
{
    // Pre-condition
	if(R == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to getNibble\n");
		return;
	}
    if( i < 0 || i > 7)
	{
		fprintf(stderr,"Error: Invalid Nibble selection\n");
		return;
	}

	//copies the initial content into an integer to compare it to in the end
	int beg = R->content;

    //copies the content into an integer value to not change the content intself
	//int content = R->content;

	//creates a mask with 4 ones that are shifted as many nibbles up as declared
	int mask = 0x0000000F << (i*4);

	//ANDs that mask with the content, and shifts the nibble to the front of the 32 bits.
    int nib = (R->content & mask) >> (i*4);
    return nib;

    // Post-condition
	if(R->content != beg)
	{
		fprintf(stderr, "Error: Failed to get Nibble\n");
		return;
	}
}

char *reg2str(iRegister r){

	// pre-condition

	char *bitArray = (char *)malloc(33 * sizeof(char));
	int j = 0;

	for (unsigned int i = 1U << 31; i > 0; i /= 2){ // shifting a mask from 1000 0000 0000 0000 0000 0000 0000 0000 -> 0*31 1
		if (r.content & i){
			bitArray[j] = '1';
		}
		else{
			bitArray[j] = '0';
		}
		j++;
	}

	bitArray[32] = '\0'; // Null-terminate, printf purposes

	// post-conditions
	// iRegister instance stays unaltered since it's passed by value, not by reference
	if (j != 32) {
        fprintf(stderr, "Error: Incorrect length of the bit string. Expected 32 but got %d\n", j);
        free(bitArray);
        return NULL;
    }

	return bitArray;

}

void shiftRight(int i, iRegister *R)
{
    // Pre-condition
    if(R == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to shiftRight\n");
		return;
	}
	if( i < 0 )
	{
		fprintf(stderr,"Error: Invalid Shift number\n");
		return;
	}

    R->content = R->content >> i;

	// Post-conditions
	for(int j = (32-i); j < 32; j++){
        if(getBit(j,&R) != 0){
            fprintf(stderr, "Error: Failed to shift right\n");
            return;
        }
	}

}

void shiftLeft(int i, iRegister *r){

	// pre-condition
	if (r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to shiftLeft\n");
	}
	if (!(i >= 0)){
		fprintf(stderr, "Error: value of i invalid\n");
	}

	r->content = r->content << i;

	// post-conditions
	// Check that i bits to the right are 0's
	int mask = (1 << i) - 1;
	if ((r->content & mask) != 0) {
		fprintf(stderr, "Error: Bits to the right are not 0\n");
		return;
	}
}

void resetBit(int i, iRegister *r)
{
	// pre-condition
	if(r == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to resetBit\n");
		return;
	}

	if( i < 0 || i > 31)
	{
		fprintf(stderr,"Error: Invalid bit\n");
		return;
	}

  	r->content &= ~(1 << i);

	// post-condition
	if((r->content & (1<<i)) != 0)
	{
		fprintf(stderr, "Error: Failed to reset Bit\n");
		return;
	}
}
