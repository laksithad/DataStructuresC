/*
 -------------------------------------------------------
 stack.c
 Linked version of the Stack ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"

// Functions

stack *stack_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string) {
	stack *s = malloc(sizeof *s);
	assert(s != NULL);

	s->top = NULL;
	s->destroy = destroy;
	s->to_string = to_string;
	s->copy = copy;
	return s;
}

int stack_empty(const stack *s) {
	return s->top == NULL;
}

int stack_full(const stack *s) {
	return 0;
}

void stack_push(stack *s, const data *value) {
	stack_node *node = malloc(sizeof *node);
	assert(node != NULL);

	node->value = s->copy(value);
	node->next = s->top;
	s->top = node;
	return;
}

data *stack_peek(const stack *s) {
	assert(s->top != NULL);

	return s->copy(s->top->value);
}

data *stack_pop(stack *s) {
	assert(s->top != NULL);

	data *value = s->top->value;
	stack_node *node = s->top;
	s->top = s->top->next;
	free(node);
	return value;
}

void stack_print(const stack *s) {
	char string[DATA_STRING_SIZE];
	stack_node *current = s->top;

	while (current != NULL) {
		printf("%s\n", s->to_string(string, DATA_STRING_SIZE, current->value));
		current = current->next;
	}
	return;
}

/**
 * Moves the top node of a source stack to a target stack.
 * @param target pointer to the target stack.
 * @param source pointer to the source stack.
 */
static void stack_move_top(stack *target, stack *source) {
	assert(source->top != NULL);

	stack_node *node = source->top;
	source->top = source->top->next;
	node->next = target->top;
	target->top = node;
	return;
}

void stack_combine(stack *target, stack *source1, stack *source2) {

	while (source1->top != NULL && source2->top != NULL) {
		stack_move_top(target, source1);
		stack_move_top(target, source2);
	}
	while (source1->top != NULL) {
		stack_move_top(target, source1);
	}
	while (source2->top != NULL) {
		stack_move_top(target, source2);
	}
	return;
}

void stack_split_alt(stack *target1, stack *target2, stack *source) {
	int left = 1;

	while (source->top != NULL) {

		if (left) {
			stack_move_top(target1, source);
		} else {
			stack_move_top(target2, source);
		}
		left = !left;
	}
	return;
}

void stack_destroy(stack **s) {
	stack_node *node = NULL;

	while ((*s)->top != NULL) {
		node = (*s)->top;
		(*s)->top = (*s)->top->next;
		(*s)->destroy(&node->value);
		free(node);
	}
	free(*s);
	*s = NULL;
	return;
}
