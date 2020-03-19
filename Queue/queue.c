/*
 -------------------------------------------------------
 queue.c
 Circular Array-based version of the Queue ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#include <stdlib.h>

#include "queue.h"

// Functions

void queue_initialize(queue *q, data_string to_string, data_destroy destroy) {
	q->capacity = QUEUE_SIZE;
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	q->to_string = to_string;
	q->destroy = destroy;
	q->values = malloc(QUEUE_SIZE * sizeof *q->values);
	return;
}

void queue_destroy(queue *q) {
	// Free up the data array.
	if (q->values != NULL) {

		for (int i = 0; i < q->capacity; i++) {
			q->destroy(&q->values[i]);
		}
		free(q->values);
		q->values = NULL;
	}
	q->capacity = 0;
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	return;
}

int queue_empty(const queue *q) {
	return (q->size == 0);
}

int queue_full(const queue *q) {
// Assume queue is not full.
	return 0;
}

int queue_count(const queue *q) {
	return (q->size);
}

void queue_insert(queue *q, data v) {

	if (q->size == q->capacity) {
		// Double the queue capacity.
		q->capacity <<= 1;

		q->values = realloc(q->values, sizeof(data) * q->capacity);
		// Must be able to allocate more memory to queue.
		assert(q->values != NULL);

		// Move values before front index into new memory
		memcpy(&q->values[q->size], q->values, sizeof(data) * q->front);
		// Move q->rear to new location after q->front.
		q->rear = q->size + q->front;
	}
	if (q->size == 0) {
		// Update the front index if necessary.
		q->front = q->rear;
	}
	// Add the new value to the values array.
	// Update rear to point to the next insertion location.
	q->values[q->rear] = v;
	q->size++;
	q->rear = (q->rear + 1) % q->capacity;
	return;
}

data queue_peek(const queue *q) {
	// Cannot peek on an empty queue.
	assert(q->front != -1);

	return (q->values[q->front]);
}

data queue_remove(queue *q) {
	// Cannot remove on an empty queue.
	assert(q->front != -1);

	data v = q->values[q->front];
	q->size--;

	if (q->size == 0) {
		q->front = 0;
		q->rear = 0;
	} else {
		q->front = (q->front + 1) % q->capacity;
	}
	return (v);
}

void queue_print(const queue *q) {
// Iterate through the queue.
	char string[80];
	int j = q->front;

	for (int i = 0; i < q->size; i++) {
		printf("%s\n", q->to_string(string, 80, &(q->values[j])));
		j = (j + 1) % q->capacity;
	}
	printf("\n");
	return;
}

void queue_combine(queue *target, queue *source1, queue *source2) {

	while (source1->size > 0 && source2->size > 0) {
		queue_insert(target, queue_remove(source1));
		queue_insert(target, queue_remove(source2));
	}
	while (source1->size > 0) {
		queue_insert(target, queue_remove(source1));
	}
	while (source2->size > 0) {
		queue_insert(target, queue_remove(source2));
	}
	return;
}

void queue_split(queue *target1, queue *target2, queue *source) {
	int left = 1;

	while (source->size > 0) {

		if (left) {
			queue_insert(target1, queue_remove(source));
		} else {
			queue_insert(target2, queue_remove(source));
		}
		left = !left;
	}
	return;
}
