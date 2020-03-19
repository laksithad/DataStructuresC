/*
 -------------------------------------------------------
 stack_array.c
 Array-based version of the Stack ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */

#include "stack.h"

// Functions

void stack_initialize(stack *s, data_string to_string, data_destroy destroy) {
	s->top = -1;
	s->to_string = to_string;
	s->destroy = destroy;
	return;
}

void stack_destroy(stack *s) {
	int i;

	for (i = 0; i < s->top; i++) {
		// Destroy individual array elements (in case they are malloc'd)
		s->destroy(&s->values[i]);
	}
	// Free up the data array.
	s->top = -1;
	return;
}

int stack_empty(const stack *s) {
	return (s->top == -1);
}

int stack_full(const stack *s) {
	// s->top cannot equal or exceed type maximum.
	return (s->top == (STACK_SIZE - 1));
}

int stack_count(const stack *s) {
	// s->top maximum must be type maximum - 1 for this to work.
	return (s->top + 1);
}

void stack_push(stack *s, data v) {
	// Cannot increase size of stack beyond the signed int maximum.
	// Must have room for top+1 as count value.
	assert(s->top < (STACK_SIZE -1));
	s->top += 1;

	// Add the new data to the values array.
	s->values[s->top] = v;
	return;
}

data stack_peek(const stack *s) {
	// Cannot peek on an empty stack.
	assert(s->top != -1);

	return (s->values[s->top]);
}

data stack_pop(stack *s) {
	// Cannot pop on an empty stack.
	assert(s->top != -1);

	data v = s->values[s->top];
	s->top--;
	return (v);
}

void stack_print(stack *s) {
	// Iterate through the stack.
	char string[MAX_LINE];

	for (int i = s->top; i >= 0; i--) {
		printf("%s\n", s->to_string(string, MAX_LINE, &(s->values[i])));
	}
	printf("\n");
	return;
}

/**
 * Moves the top node of a source stack to a target stack.
 * @param target pointer to the target stack.
 * @param source pointer to the source stack.
 */
static void stack_move_top(stack *target, stack *source) {
	// Cannot move the top of an empty stack.
	assert(source->top != -1);

	stack_push(target, stack_pop(source));
	return;
}

void stack_combine(stack *target, stack *source1, stack *source2) {

	while (source1->top != -1 && source2->top != -1) {
		stack_move_top(target, source1);
		stack_move_top(target, source2);
	}
	while (source1->top != -1) {
		stack_move_top(target, source1);
	}
	while (source2->top != -1) {
		stack_move_top(target, source2);
	}
	return;
}

void stack_split(stack *target1, stack *target2, stack *source) {

	while (source->top != -1) {

		if (source->top % 2 == 0) {
			stack_move_top(target1, source);
		} else {
			stack_move_top(target2, source);
		}
	}
	return;
}
