/*
 -------------------------------------------------------
 priority_queue.h
 Linked version of the Priority Queue ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

// Define and declare the data type
#include "data.h"

/**
 * Priority Queue node.
 */
typedef struct pq_node {
	data *value; ///< Pointer to the node data.
	struct pq_node *next; ///< Pointer to the next priority queue node.
} pq_node;

/**
 * Priority Queue header.
 */
typedef struct {
	int size; ///< Number of values in the priority queue.
	pq_node *front; ///< Pointer to the first node of the priority queue.
	pq_node *rear; ///< Pointer to rear node of the priority queue.
	data_destroy destroy; ///< Pointer to data destroy function.
	data_copy copy; ///< Pointer to data copy function.
	data_to_string to_string; ///< Pointer to data to string function.
	data_compare compare; ///< Pointer to data comparison function.
} priority_queue;

// Prototypes

/**
 * Initializes a priority queue structure.
 * @param destroy The destroy function for the priority queue data.
 * @param copy The copy function for the priority queue data.
 * @param to_string The to string function for the priority queue data.
 * @param data_compare The comparison function for the priority queue data.
 * @return a pointer to a new priority queue.
 */
priority_queue *pq_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare);

/**
 * Destroys a priority queue.
 * @param pq Pointer to a priority queue.
 */
void pq_destroy(priority_queue **pq);

/**
 * Determines if a priority queue is empty.
 * @param pq Pointer to a priority queue.
 * @return
 */
int pq_empty(const priority_queue *pq);

/**
 * Determines if the priority queue is full.
 * @param pq Pointer to a priority queue.
 * @return 1 if the priority queue is full, 0 otherwise.
 */
int pq_full(const priority_queue *pq);

/**
 * Returns the number of elements in the priority queue.
 * @param pq Pointer to a priority queue.
 * @return The number of values in the priority queue.
 */
int pq_size(const priority_queue *pq);

/**
 * Inserts a copy of value into the priority queue. Values are stored in priority order.
 * @param pq Pointer to a priority queue.
 * @param value Value to insert into the priority queue.
 */
void pq_insert(priority_queue *pq, data *value);

/**
 * Returns a copy of the highest priority value in a priority queue,
 * the priority queue is unchanged.
 * @param pq Pointer to a priority queue.
 * @return Pointer to a copy of the value in the front of the priority queue.
 */
data *pq_peek(const priority_queue *pq);

/**
 * Returns and removes the highest priority value in the priority queue.
 * @param pq Pointer to a priority queue.
 * @return Pointer to the value removed from the front of the priority queue.
 */
data *pq_remove(priority_queue *pq);

/**
 * Prints the elements in a priority queue from front to rear.
 * @param pq Pointer to a priority queue.
 */
void pq_print(const priority_queue *pq);

/**
 * Combines the contents of source1 and source2 into target.
 * source1 and source2 are left empty.
 * @param target pointer to destination priority queue
 * @param source1 pointer to first source priority queue
 * @param source2 pointer to second source priority queue
 */
void pq_combine(priority_queue *target, priority_queue *source1,
		priority_queue *source2);

/**
 * Splits the contents of source into target1 and target2, values of source
 * alternating between target1 and target2. source is left empty.
 * @param target1 pointer to first destination priority queue
 * @param target2 pointer to second destination priority queue
 * @param source pointer to source queue
 */
void pq_split_alt(priority_queue *target1, priority_queue *target2,
		priority_queue *source);

/**
 * Splits the contents of source into higher and low_equal queues according to
 * value of key.
 * higher and lower must both start empty.
 * source is left empty.
 * @param higher pointer to queue containing values with higher priority than key
 * @param low_equal pointer to queue containing values with lower or equal priority to key
 * @param source pointer to source queue
 * @param key data value to split source on
 */
void pq_split_key(priority_queue *higher, priority_queue *low_equal,
		priority_queue *source, data *key);

#endif /* PRIORITY_QUEUE_H_ */
