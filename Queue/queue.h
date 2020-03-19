/*
 -------------------------------------------------------
 queue_array.h
 Array-based version of the Queue ADT.
 -------------------------------------------------------
 Author:       David Brown
 ID:           987654321
 Email:        dbrown@wlu.ca
 Version:      2019-05-31
 -------------------------------------------------------
 */
#ifndef QUEUE_H_
#define QUEUE_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "data.h"

// Structures
/**
 * Initial stack size.
 */
#define QUEUE_SIZE 8

/**
 * Queue header.
 */
typedef struct {
	unsigned int capacity; ///< current capacity of the queue.
	int front; ///< index of front element of the queue.
	int rear; ///< index of next rear element of the queue.
	int size; ///< count of number of values in the queue.
	data_string to_string; ///< prints stack data element
	data_destroy destroy; ///< destroys stack data element
	data *values; ///< array of data values.
} queue;

// Prototypes

/**
 * Initializes a queue structure.
 * @param q the queue to initialize.
 */
void queue_initialize(queue *q, data_string to_string, data_destroy destroy);

/**
 * Destroys a queue.
 * @param q pointer to a queue.
 */
void queue_destroy(queue *q);

/**
 * Determines if a queue is empty.
 * @param q pointer to a queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int queue_empty(const queue *q);

/**
 * Determines if the queue is full.
 * @param q pointer to a queue.
 * @return 1 if the queue is full, 0 otherwise.
 */
int queue_full(const queue *q);

/**
 * Returns the number of elements in the queue.
 * @param s pointer to a queue.
 * @return the number of elements in the queue.
 */
int queue_count(const queue *q);

/**
 * Adds a value to the rear of a queue.
 * @param q pointer to a queue.
 * @param v the value to insert.
 */
void queue_insert(queue *q, data v);

/**
 * Returns the value on the front of a queue, the queue is unchanged.
 * @param q pointer to a queue.
 * @return the value at the front of the queue.
 */
data queue_peek(const queue *q);

/**
 * Returns and removes the value on the front of a queue.
 * @param q pointer to a queue.
 * @return the value at the front of the queue, value is removed
 */
data queue_remove(queue *q);

/**
 * Prints the elements in a queue from front to rear.
 * (For testing only).
 * @param q pointer to a queue.
 */
void queue_print(const queue *q);

/**
 * Combines the contents of two source queues into a target queue -
 * the source queues are empty when the function completes (iterative).
 * @param target pointer to the target queue.
 * @param source1 pointer to the first source queue.
 * @param source2 pointer to the second source queue.
 */
void queue_combine(queue *target, queue *source1, queue *source2);

/**
 * Splits a source queue alternately into two target queues - the source
 * queue is empty when the function completes (iterative).
 * @param target1 pointer to the first target queue.
 * @param target2 pointer to the second target queue.
 * @param source pointer to the source queue.
 */
void queue_split(queue *target1, queue *target2, queue *source);

/**
 * Compares two queues. Returns 1 if they contain the same data in the same order,
 * 0 otherwise.
 * @param q1 the first queue to compare.
 * @param q2 the second queue to compare.
 * @return
 */
int queue_identical(queue *q1, queue *q2);

#endif /* QUEUE_H_ */
