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
    if(~(R->content & (1<<i)) != 0)
	{
		fprintf(stderr, "Error: Failed to set Bit\n");
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


int getNibble(int i, iRegister *R)
{
    // Pre-condition
	if(R == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to getNibble\n");
		return;
	}
    if( i < 1 || i > 8)
	{
		fprintf(stderr,"Error: Invalid Nibble selection\n");
		return;
	}

	//copies the initial content into an integer to compare it to in the end
	int beg = R->content;

    //copies the content into an integer value to not change the content intself
	int con = R->content;

	//creates a mask with 4 ones that are shiftesd as many nibbles up as declared
	int mask = 00001111 << ((i-1)*4);

	//ANDs that mask with the content, and shifts the nibble to the front of the 32 bits.
    int nib = (con &= mask) >> ((i-1)*4);
    return nib;

    // Post-condition
	if(R->content != beg)
	{
		fprintf(stderr, "Error: Failed to get Nibble\n");
		return;
	}
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

	//shifts the content right by i places
    R->content = R->content >> i;

	// Post-conditions
	for(int j = (32-i); j < 32; j++){
        if(getBit(j,&R) != 0){
            fprintf(stderr, "Error: Failed to shift right\n");
            return;
        }
	}

	if((R->content << i) != R->content){
        fprintf(stderr, "Error: Failed to shift right\n");
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
