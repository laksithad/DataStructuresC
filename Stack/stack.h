/*
 -------------------------------------------------------
 stack_array.h
 Array-based version of the Stack ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#ifndef STACK_ARRAY_H_
#define STACK_ARRAY_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"

// Structures
/**
 * Initial stack size.
 */
#define STACK_SIZE 22

/**
 * Stack header.
 */
typedef struct {
	int top; ///< index of top element of the stack.
	data values[STACK_SIZE]; ///< array of data values.
	data_string to_string; ///< prints stack data element
	data_destroy destroy; ///< destroys stack data element
} stack;

// Prototypes

/**
 * Initializes a stack structure.
 * @param s the stack to initialize.
 * @param print data print function
 * @param destroy data destroy function
 */
void stack_initialize(stack *s, data_string to_string, data_destroy destroy);

/**
 * Destroys a stack.
 * @param s pointer to a stack.
 */
void stack_destroy(stack *s);

/**
 * Determines if a stack is empty.
 * @param s pointer to a stack.
 * @return 1 if the stack is empty, 0 otherwise.
 */
int stack_empty(const stack *s);

/**
 * Determines if the stack is full.
 * @param s pointer to a stack.
 * @return 1 if the stack is full, 0 otherwise.
 */
int stack_full(const stack *s);

/**
 * Returns the number of elements in the stack.
 * @param s pointer to a stack.
 * @return the number of elements in the stack.
 */
int stack_count(const stack *s);

/**
 * Pushes a value onto a stack.
 * @param s pointer to a stack.
 * @param v the value to push.
 */
void stack_push(stack *s, data v);

/**
 * Returns the value on the top of a stack, the stack is unchanged.
 * @param s pointer to a stack.
 * @return a pointer to the value on the top of the stack.
 */
data stack_peek(const stack *s);

/**
 * Returns and removes the value on the top of a stack.
 * @param s pointer to a stack.
 * @return the value on the top of the stack, value is removed from stack
 */
data stack_pop(stack *s);

/**
 * Prints the elements in a stack from top to bottom.
 * (For testing only).
 * @param s pointer to a stack.
 */
void stack_print(stack *s);

/**
 * Combines the contents of two source stacks into a target stack -
 * the source stacks are empty when the function completes (iterative).
 * @param target pointer to the target stack.
 * @param source1 pointer to the first source stack.
 * @param source2 pointer to the second source stack.
 */
void stack_combine(stack *target, stack *source1, stack *source2);

/**
 * Splits a source stack alternately into two target stacks - the source
 * stack is empty when the function completes (iterative).
 * @param target1 pointer to the first target stack.
 * @param target2 pointer to the second target stack.
 * @param source pointer to the source stack.
 */
void stack_split(stack *target1, stack *target2, stack *source);

#endif /* STACK_ARRAY_H_ */
