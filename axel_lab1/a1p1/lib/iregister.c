//  Created by Mohammadreza Mousavi [mohmou] on 9/5/14.
//  Updated by Masoumeh Taromirad on 11/08/16.
//  Updated by Wagner Morais and Johannes van Esch on 28/08/18.
//  Updated by Wagner Morais and Hazem Ali on 26/08/21.
//  Copyright (c) 2014 by Mohammadreza Mousavi [mohmou]. All rights reserved.
 
#include <stdlib.h>
#include <stdio.h>
#include "iregister.h"

void resetBit(int i, iRegister *r)
{
	// pre-condition
	if(r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to resetBit\n");
		return;
	}
	// pre-condition
	if( i < 0 || i > 31){
		fprintf(stderr,"Error: Invalid bit\n");
		return;
	}
	
  	r->content &= ~(1 << i);

	// post-condition
	if((r->content & (1<<i)) != 0){
		fprintf(stderr, "Error: Failed to reset Bit\n");
		return;	
	}
}

void resetAll(iRegister *r)
{
	// pre condition
	if (r == NULL){
		fprintf(stderr, " Error: A NULL pointer was given to resetAll\n");
		return;
	}

	r->content = 0;	

	// post-condition
	if((r->content != 0)){
		fprintf(stderr, "Error: Failed to reset all\n");
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
	if((r->content != 0)){
		fprintf(stderr, "Error: Failed to set all bits to 1\n");
		return;	
	}
}

void assignNibble(int a, int b, iRegister *r){

	// pre-conditions
	if (r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to setAll\n");
		return;
	}
	if (a < 0 || a > 15 || b < 0 || b > 15){
		fprintf(stderr, "Error: Invalid a or b\n");
	}

	// clear bits 0-7
	r->content &= ~255;  // ~255 = 1111 1111 1111 1111 1111 1111 0000 0000
	// add a
	r->content += a;
	// // add b
	r->content += (b << 4);

	// post-condition how? 
}

char *reg2str(iRegister r){

	// pre-condition
	if (&r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to setAll\n");
	}

	char bitArray[32];
	int j = 0;
	for (int i = 1 << 31; i > 0; i /= 2){ // shifting a mask from 1000 0000 0000 0000 0000 0000 0000 0000 -> 0*31 1
		if (r.content & i){
			bitArray[j] = '1';
		}
		else{
			bitArray[j] = '0';
		}
		j++;
	}
	for (int k = 0; k < 32; k++) {
        printf("%c", bitArray[k]); // what in black magic ? 
    }
	// return bitArray; memory stuff I need to read up on

}

int main(){
	iRegister a; 
	a.content = 2649;

	reg2str(a);


	return 0;
	// int c = 2649 & ~255; 
	// int d = c + 5; 
	// int e = d + (3 << 4);
	// printf("%d\n", e);
	// assignNibble(5, 3, &a);
	// printf("%d\n", a.content);
// 	printf("%d\n", a.content);
// 	setAll(&a);
// 	printf("%d\n", a.content);
}