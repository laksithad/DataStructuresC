/*
 -------------------------------------------------------
 priority_queue.c
 Linked version of the Priority Queue ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
// Includes
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "priority_queue.h"

priority_queue *pq_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare) {
	priority_queue *pq = malloc(sizeof *pq);
	assert(pq != NULL);

	pq->front = NULL;
	pq->rear = NULL;
	pq->size = 0;
	pq->copy = copy;
	pq->destroy = destroy;
	pq->to_string = to_string;
	pq->compare = compare;
	return pq;
}

int pq_empty(const priority_queue *pq) {
	return pq->front == NULL;
}

int pq_full(const priority_queue *pq) {
	return 0;
}

int pq_size(const priority_queue *pq) {
	return pq->size;
}

void pq_insert(priority_queue *pq, data *value) {
	pq_node *prev = NULL;
	pq_node *curr = pq->front;

	// Loop through the linked list to find the proper position for value.
	while (curr != NULL && pq->compare(curr->value, value) > 0) {
		prev = curr;
		curr = curr->next;
	}
	// Create the new node - add copy of data, link to curr.
	pq_node *new_node = malloc(sizeof *new_node);
	assert(new_node != NULL);

	new_node->value = pq->copy(value);
	new_node->next = curr;
	pq->size++;

	if (pq->front == NULL) {
		// New node is the only node in the queue
		pq->front = new_node;
		pq->rear = new_node;
	} else if (prev == NULL) {
		// New node is the first node in the queue.
		pq->front = new_node;
	} else if (curr == NULL) {
		// New node is the new rear node.
		pq->rear->next = new_node;
		pq->rear = new_node;
	} else {
		// New node is anywhere else in the queue.
		prev->next = new_node;
	}
	return;
}

data *pq_peek(const priority_queue *pq) {
	assert(pq->front != NULL);

	return pq->copy(pq->front->value);
}

data *pq_remove(priority_queue *pq) {
	assert(pq->front != NULL);

	data *value = pq->front->value;
	pq_node *temp = pq->front;
	pq->front = pq->front->next;
	pq->size -= 1;
	free(temp);

	if (pq->front == NULL) {
		// Last node is removed
		pq->rear = NULL;
	}
	return value;
}

void pq_print(const priority_queue *pq) {
	char string[DATA_STRING_SIZE];
	pq_node *curr = pq->front;

	while (curr != NULL) {
		printf("%s\n", pq->to_string(string, DATA_STRING_SIZE, curr->value));
		curr = curr->next;
	}
	return;
}

void pq_destroy(priority_queue **pq) {
	pq_node *temp = NULL;

	while ((*pq)->front != NULL) {
		temp = (*pq)->front;
		(*pq)->front = (*pq)->front->next;
		(*pq)->destroy(&temp->value);
		free(temp);
	}
	free(*pq);
	*pq = NULL;
	return;
}

/**
 * Moves the front node from source to the rear of target.
 * @param target pointer to target queue
 * @param source pointer to source queue
 */
static void move_front_to_rear(priority_queue *target, priority_queue *source) {
	assert(source->front !=NULL);

	pq_node *temp = source->front;
	// Update the source queue.
	source->front = source->front->next;
	source->size--;

	if (source->front == NULL) {
		source->rear = NULL;
	}
	// Update the target queue.
	if (target->front == NULL) {
		target->front = temp;
	} else {
		target->rear->next = temp;
	}
	target->rear = temp;
	target->rear->next = NULL;
	target->size++;
	return;
}

/**
 * Adds the entire source queue to the end of the target queue.
 * The source queue is empty at the end of the function.
 * @param target Pointer to the target queue
 * @param source Pointer to the source queue
 */
static void append_queue(priority_queue *target, priority_queue *source) {
	// Update the target queue.
	if (target->front == NULL) {
		target->front = source->front;
	} else {
		target->rear->next = source->front;
	}
	target->rear = source->rear;
	target->size += source->size;
	// Empty the source queue
	source->front = NULL;
	source->rear = NULL;
	source->size = 0;
	return;
}

void pq_combine(priority_queue *target, priority_queue *source1,
		priority_queue *source2) {

	while (source1->front != NULL && source2->front != NULL) {
		// Move higher priority nodes into target to preserve order.
		if (target->compare(source2->front->value, source1->front->value) < 0) {
			move_front_to_rear(target, source1);
		} else {
			move_front_to_rear(target, source2);
		}
	}
	// Empty remaining source queue.
	if (source1->front != NULL) {
		append_queue(target, source1);
	}
	if (source2->front != NULL) {
		append_queue(target, source2);
	}
	return;
}

void pq_split_alt(priority_queue *target1, priority_queue *target2,
		priority_queue *source) {
	int left = 1;

	while (source->front != NULL) {
		if (left) {
			move_front_to_rear(target1, source);
		} else {
			move_front_to_rear(target2, source);
		}
		left = !left;
	}
	return;
}

void pq_split_key(priority_queue *higher, priority_queue *low_equal,
		priority_queue *source, data *key) {
	pq_node *prev = NULL;
	pq_node *curr = source->front;
	int count = 0;

	while (curr != NULL && source->compare(curr->value, key) > 0) {
		// Find the location to split the queue.
		prev = curr;
		curr = curr->next;
		count++;
	}
	if (curr != NULL) {
		// Define the queue with lower priority values
		low_equal->front = curr;
		low_equal->rear = source->rear;
	}
	if (prev != NULL) {
		// Define the queue of higher priority values
		higher->front = source->front;
		prev->next = NULL;
		higher->rear = prev;
	}
	// Update the sizes
	low_equal->size = source->size - count;
	higher->size = count;
	// Empty the source queue
	source->size = 0;
	source->front = NULL;
	source->rear = NULL;
	return;
}
