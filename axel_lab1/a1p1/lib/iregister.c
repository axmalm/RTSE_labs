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
	if((r->content != -1)){
		fprintf(stderr, "Error: Failed to set all bits to 1\n");
		return;	
	}
}

void assignNibble(int a, int b, iRegister *r){

	// pre-conditions
	if (r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to assignNibble\n");
		return;
	}
	if (a < 0 || a > 15 || b < 0 || b > 15){
		fprintf(stderr, "Error: Invalid a or b\n");
		return;
	}

	int content_before = r->content;

	// clear bits 0-7
	r->content &= ~255;  // ~255 = 1111 1111 1111 1111 1111 1111 0000 0000
	// add a
	r->content += a;
	// add b
	r->content += (b << 4);

	// post-condition
	// Check bits 0-3
	if ((r->content & 15) != a) { // bitmask 0000 0000 0000 0000 0000 0000 0000 1111 
		fprintf(stderr, "Error: Bits 0-3 does not match '%d'\n", a);
		return;
	}
	// Check bits 4-7
	if (((r->content >> 4) & 15) != b){
		fprintf(stderr, "Error: Bits 4-7 does not match '%d'\n", b);
		return;
	}
	// Check bits 8-31
	if ((r->content & (-1 & ~255)) != (content_before & (-1 & 255))){
		fprintf(stderr, "Error: Bits 8-31 has ben altered", b);
		return;
	}
}

char *reg2str(iRegister r){

	// pre-condition

	char *bitArray = (char *)malloc(33 * sizeof(char)); // debug 33
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

void shiftLeft(int i, iRegister *r){

	// pre-condition
	if (r == NULL){
		fprintf(stderr, "Error: A NULL pointer was given to shiftLeft\n");
	}
	if (!(i >= 0)){
		fprintf(stderr, "Error: value of i invalid\n");
	}

	for (int j = 0; j < i; j++){
		r->content <<= 1;
	}

	// post-condition
	// Check i bits to the right are 0's
	// for (int j = 32 - i; j <= 31; j++){
	int mask = ;
	if ((r->content << j) != 0) {
		fprintf(stderr, "Error: Bits to the right are not 0\n");
		return;
	}
	
}

int main(){
	iRegister a; 
	a.content = 8;

	// char *bitArray = reg2str(a);
	// printf("%s\n", bitArray);

	shiftLeft(1, &a);
	printf("%d\n", a.content);



	return 0;
}