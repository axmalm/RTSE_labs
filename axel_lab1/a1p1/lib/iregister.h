//  Created by Mohammadreza Mousavi [mohmou] on 9/5/14.
//  Updated by Masoumeh Taromirad on 11/08/16.
//  Updated by Wagner Morais and Johannes van Esch on 28/08/18.
//  Copyright (c) 2014 by Mohammadreza Mousavi [mohmou]. All rights reserved.

#ifndef lab0_iregister_h
#define lab0_iregister_h

/**
 *  iRegister
 *  An iRegister is a structure which represents an 32-bit register and
 *  is equipped with standard operations to modify and display them.
 */ 
typedef struct{
    int content;
} iRegister;

/**
 *  Bellow you find the declarations for the functions to modify and display the
 *  memory content of a iRegister data structure. Before each declaration, a brief 
 *  description about what the function shall do is given. 
 *  Later in this file, the documentation for the resetBit function is given. 
 *  Students should follow that format.
 */ 

/** resets all the bits of the iRegister (to 0)
 * 
 * parameters: a pointer to an iRegister instance
 * 
 * return: no return value, just modifies an existing iRegister instance
 * 
 * pre-condition: an instance of the iRegister struct has been initialized
 * 
 * post-condition: the instance of the iRegister struct will be all zeroes
 * 
 * properties: after resetAll(), getBIT(x) returns 0 for all x
 * 
 */ 
void resetAll(iRegister *);

/** sets the i'th bit of the iRegister (to 1)
 * 
 * parameters: an integer which declares which bit that should be set to 1
 *             a pointer to an iRegister instance
 * 
 * return: void
 * 
 * pre-conidion: 0 <= i < 32 and iRegister != Null
 * 
 * post-condition: after the setBit function, the i'th bit is set to 1, all other bits remain unchanged
 * 
 * properties: after setBit(i, r),  getBit(i, r) = 1
 * if setBit(i, R), then getBit(j, R) should return the same values as before setBit(i,R) for all j != i
 * 
 */
void setBit(int, iRegister *);


/**sets all the bits of the iRegister (to 1)
 * 
 * parameters: a pointer to an iRegister instance
 * 
 * return: void
 * 
 * pre-condition: iRegister != NULL
 * 
 * post-condition: after setAll function, all bits in the register are set to 1
 * 
 * properties: getBit(i, R), for all i, returns 1
 * 
 */
void setAll(iRegister *);


/** returns the i'th bit of the iRegister as an integer (1 if it is set, or 0 otherwise)
 * 
 * parameters: an integer, either 1 or 0
 *             a pointer to an iRegister instance
 * 
 * return: the value of the bit at position i
 * 
 * pre-condition: 0 <= i < 32 and iRegister != Null
 * 
 * post-condition: After getBit(i,R), the R stays the same, and the value of the bit at position i is returned
 * 
 * properties: getBit(j,R) is the same values for j before and after getBit(i,R) has been called
 * the return value is either a 1 or 0
 */
int getBit(int, iRegister *);


/** set the first (for pos=1) or the second (for pos=2) four bits of iRegsiter
 * 
 * parameters: the input parameter a should describe the value of the first 4 bits, while input b describes the value of the second four bits
 *             r: a pointer to a memory location of an iRegister instance
 * 
 * pre-condition: integer a and b = [0, 15] and iRegister != NULL
 * 
 * post-condition: after assignNibble(a, b, R), the bits (0-3) will be assigned as 1s or 0s according to a
 *                 after assignNibble(a, b, R), the bits (4-7) will be assigned as 1s or 0s according to b 
 *                 the bits (8-31) will remain unchanged
 *
 * properties: getBit(j,R) is the same as before assignNibble if 7 < j < 31. 
 */
void assignNibble(int, int, iRegister *);


/** get the first (for pos=1) or the second (for pos=2) four bits of iRegsiter
 * 
 * parameters: chooses the nibble to be returned
 *             a pointer to a memory location of an iRegister instance
 * 
 * return: an integer between 0-15, that corresponds to the nibbles either in position 1 or 2
 * 
 * pre-condition: 1 <= i <= 2 and iRegister != NULL
 * 
 * post-condition: the iRegister instance stays unaltered
 *                 the return is the i'th nibble of the iRegister as an integer value
 * 
 * properties: getBit(i,R) returns the same value, for all i, before and after getNibble is called
 */
int getNibble(int, iRegister *);


/** returns a pointer to an array of 32 characters, with each character 
 *  representing the corresponding bit of the iRegister, i.e., if the bit is set,
 *  then the character is "1" (ASCII char with code 49), or otherwise is "0" 
 *  (ASCII char with code 48)
 * 
 * parameters: an instance of an iRegister struct
 * 
 * return: a pointer to an array of characters
 * 
 * pre-condition: iRegister != NULL
 * 
 * post-condition: the iRegister instance stays unaltered
 *                 a new array of 32 characters is initialized with the same corresponding values as the iRegister instance, but in char
 * 
 * properties: getBit(i,R) returns the same value, for all i, before and after *reg2str is called´
 *             if getBit(x,R) == 0, and c = char *reg2str, then *C[x] = "0"
 */
char *reg2str(iRegister);


/** shifts all the bits of the iRegister to the right by n palces (appends 0 
 *  from the left)
 * 
 * parameters: an integer declaring number of shifts to the right'
 *             a pointer to a memory location of an iRegister instance
 * 
 * return: void
 * 
 * pre-condition: i >= 0
 *                iRegister != NULL
 * 
 * post-condition: the bits are shifted i places to the right
 *                 i number of 0s are appended from the left
 * 
 * properties: getBit(i, R) = getBit(i+n, R) after shiftRight(n, R)
 *             if shiftRight(x, R), then getBit(j, R) == 0 for all j < i
 */
void shiftRight(int, iRegister *);


/** shifts all the bits of the iRegister to the left by n palces (appends 0 
 *  from the right)
 * 
 * parameters: an integer declaring number of shifts to the left
 *             a pointer to a memory location of an iRegister instance
 * 
 * return: void
 * 
 * pre-condition: i >= 0
 *                iRegister != NULL
 * 
 * post-condition: the bits are shifted i places to the left
 *                 i number of 0s are appended from the right
 * 
 * properties: getBit(i, R) = getBit(i-n, R) after shiftLeft(n, R)
 *             if shiftLeft(x, R), then getBit(j, R) == 0 for all j > 31 - x
 */
void shiftLeft(int, iRegister *);


/** @brief Resets the i'th bit of the iRegister (to 0)
 *
 *  @param i Is i'th bit of the iRegister to be reset
 * 
 *  @param r A pointer to a memory location of a iRegister data structure.
 * 
 *  @return void
 * 
 *  Pre-condition: 0 <= i < 32 and iRegister != Null
 * 
 *  Post-condition: after reset(i, r) the i'th bit of iRegister is 0, all other 
 *  bits remain unchanged
 *  properties: 
 *  after resetBit(i, r),  getBit(i, r) = 0
 *  if getBit(i, r) == 0 then  
 *    getBit(j, r) returns the same value for all 
 *  0 <= j < 32 and j <> i before and after resetBit(i, r)
 * 
 *  test-cases: 
 *  1,2,3. Allocate memory to an iRegister r
 *  first do resetAll(&r),
 *  then set the i'th bit of &x by setBit(i, &r) for i = 0, 15 and 23 and then
 *  display the result after each and every call by 
 *    printf("%s",reg2str(r)) 
 */
void resetBit(int, iRegister *);

#endif
