/*
 -------------------------------------------------------
 stack.h
 Linked version of the Stack ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#ifndef STACK_H_
#define STACK_H_

// define and declare the data type
#include "data.h"

/**
 * Stack node.
 */
typedef struct stack_node {
	data *value; ///< Pointer to the node data.
	struct stack_node *next; ///< Pointer to the next stack node.
} stack_node;

/**
 * Stack header.
 */
typedef struct {
	stack_node *top; ///< Pointer to the top node of the stack.
	data_destroy destroy; ///< Pointer to data destroy function.
	data_copy copy; ///< Pointer to data copy function.
	data_to_string to_string; ///< Pointer to data to string function.
} stack;

// Prototypes

/**
 * Initializes a stack structure.
 * @param destroy The destroy function for the stack data.
 * @param copy The copy function for the stack data.
 * @param to_string The to string function for the stack data.
 * @return a pointer to a new stack.
 */
stack *stack_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string);

/**
 * Destroys a stack.
 * @param s Pointer to a stack.
 */
void stack_destroy(stack **s);

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
 * Pushes a value onto a stack.
 * @param s Pointer to a stack.
 * @param d The data to push.
 */
void stack_push(stack *s, const data *value);

/**
 * Returns a copy of the value on the top of a stack, the stack is unchanged.
 * @param s Pointer to a stack.
 * @return Pointer to a copy of the value on the top of the stack.
 */
data *stack_peek(const stack *s);

/**
 * Returns and removes the value on the top of a stack.
 * @param s Pointer to a stack.
 * @return Pointer to the value removed from the top of the stack.
 */
data *stack_pop(stack *s);

/**
 * Prints the elements in a stack from top to bottom.
 * @param s Pointer to a stack.
 */
void stack_print(const stack *s);

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
void stack_split_alt(stack *target1, stack *target2, stack *source);

#endif /* STACK_H_ */
