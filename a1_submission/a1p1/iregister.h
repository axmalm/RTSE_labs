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

/** brief description: resets all the bits of the iRegister (to 0)
 *
 * parameters: a pointer to the memory location of an iRegister data structure
 *
 * return value: this function clears the structure and doesn't return anything (void)
 *
 * Pre-condition: An instance of the iRegister type has been initialized
 *
 * Post-condition: The instance of the iRegister type will be all zeros. It will be cleared of any
 * previous values
 *
 * Properties: if resetAll(R), then getBit(x,R) == 0 for all values of x
 */
void resetAll(iRegister *);


/** brief description: sets the i'th bit of the iRegister (to 1)
 *
 * Parameters: a pointer to the memory location of an iRegister data structure, and parameter i is the i'th bit of the iRegister
 * to be reset
 *
 * return value: doesn't return anything (void)
 *
 * Pre-condition: 0 <= i < 31 and an instance of iRegister has been initialized (iRegister != Null)
 *
 * Post-condition: After the setBit function setBit(i,R), the i'th bit of R will be a one, all other bits
 * remain unchanged
 *
 * Properties: if setBit(i,R), then getBit(i,R) = 1,
 * if setBit(i,R), then getBit(j,R) should return the same values before and after the setBit
 * function for all j != i
 */
void setBit(int, iRegister *);


/**brief description: sets all the bits of the iRegister (to 1)
 *
 * parameters: a pointer to the memory location of an iRegister data structure
 *
 * return value: void
 *
 * Pre-condition: iRegister != Null
 *
 * Post-condition: After the setAll function, all bits of the iRegister should be one
 *
 * Properties: if setAll(R), then getBit(j,R) == 1 for all values of j
 */
void setAll(iRegister *);


/** brief description: returns the i'th bit of the iRegister as an integer (1 if it is set, or 0 otherwise)
 *
 * parameters: a pointer to the memory location of an iRegister data structure and and parameter i which is the i'th bit of the Register
 * to be returned
 *
 * return value: an integer of value one or zero at position i in the iRegister
 *
 * Pre-condition:  0 <= i < 32 and iRegister != Null
 *
 * Post-condition: After the getBit(i,R) the iRegister R stays exactly the same, and its i'th bit is returned
 *
 * Properties: getBit(j,R) is the same for values of j before and after the getBit(i,R) is executed
 * the returned value will be either one or zero (getBit(i,R)==0 or getBit(i,R)==1)
 */
int getBit(int, iRegister *);


/** brief description: set any four bits of iRegsiter
 *
 * Parameters:  parameter a: is the nibble to be set int iRegister
 *              parameter b: is the value assigned to the chosen nibble in iRegister
 *              parameter r: a pointer to the memory location of an iRegister data structure
 *
 * return value: void
 *
 * Pre-condition: 0 <= a <= 7 and 0 <= b <= 15 and iRegister != Null
 *
 * Post-condition: The chsoen Nibble of iRegister will be eqal to the binary value of b, all other values of iRegister will stay the same
 *
 * Properties: getBit(j,R) is the same before and after the assignNibble function when 8 <= j <= 31
 *
 */
void assignNibble(int, int, iRegister *);


/** brief description: get any four bits (Nibble) of iRegsiter
 *
 * parameters:  parameter a: chooses the nibble to be returned
 *              parameter r: a pointer to the memory location of an iRegister data structure
 *
 * return value: an integer i: 0 <= i <= 15, that corresponds to the first or second nibble in iRegister
 *
 * Pre-condition: 0 <= i <= 7 and iRegister != Null
 *
 * Post-condition: The values in the iRegister do not change and the returned value is the i'th nibble of the
 * iRegister as an integer value
 *
 * Properties:  getBit(j,R) is the exact same before and after the execution of getNibble for all values of j
 *
 */
int getNibble(int, iRegister *);


/** brief description: returns a pointer to an array of 32 characters, with each character
 *  representing the corresponding bit of the iRegister, i.e., if the bit is set,
 *  then the character is "1" (ASCII char with code 49), or otherwise is "0"
 *  (ASCII char with code 48)
 *
 * parameters: an instance of the iRegister data type
 *
 * return value: a pointer to an array of characters
 *
 * Pre-condition: iRegister != Null
 *
 * Post-condition: The values in the iRegister do not change and stay exactly the same and a new array of
 * 32 characters was created with the same corresponding values of iRegister
 *
 * Properties: getBit(j,R) is the exact same before and after the execution of *reg2str(R) for all values of j
 * if getBit(x,R)== 0, and C = char *reg2str(R), then *C[x] = "0"
 */
char *reg2str(iRegister);


/** brief description: shifts all the bits of the iRegister to the right by n palces (appends 0
 *  from the left)
 *
 * parameters:  parameter i: an integer declaring the number of shifts right
 *              parameter r: a pointer to the memory location of an iRegister data structure
 *
 * return value: void
 *
 * Pre-condition: i >= 0 and iRegister != Null
 *
 * Post-condition: the values in iRegister are shifted i places to the right, with an amount of i zeros
 * being shifted in on the left
 *
 * Properties:  if shiftRight(x,R), then getBit(j,R)==0 for all j < x
 *              getBit(i,R) = getBit(i+n,R) after shiftRight(n,R)
 */
void shiftRight(int, iRegister *);


/** brief description: shifts all the bits of the iRegister to the left by n palces (appends 0
 *  from the right)
 *
 * parameters:  parameter i: an integer declaring the number of shifts left
 *              parameter r: a pointer to the memory location of an iRegister data structure
 *
 * return value: void
 *
 * Pre-condition: i >= 0 and iRegister != Null
 *
 * Post-condition: the values in iRegister are shifted i places to the left, with an amount of i zeros
 * being shifted in on the right
 *
 * Properties:  if shiftLeft(x,R), then getBit(j,R)==0 for all j > (31-x)
 *              getBit(i,R) = getBit(i-n,R) after shiftLeft(n,R)
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
