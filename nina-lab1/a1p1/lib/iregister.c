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

	//creates the return integer A, assuming it is a one
    int A = 1;

    // creates a mask with only the i'th value being high - this is bitwise ANDed with the content. if the value is zero, the overall output changes to zero
	if(R->content &= (1<<i) == 0){
        int A = 0;
    }

    //returns the value of the i'th bit
    return A;


	// Post-condition
    if((R->content & (1<<i)) != A)
	{
		fprintf(stderr, "Error: Failed to get Bit\n");
		return;
	}
}


int getNibble(int i, iRegister *R)
{

}


void resetBit(int i, iRegister *r)
{
	// pre-condition
	if(r == NULL)
	{
		fprintf(stderr, "Error: A NULL pointer was given to resetBit\n");
		return;
	}
	// pre-condition
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
