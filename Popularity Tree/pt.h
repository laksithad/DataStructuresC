/*
 -------------------------------------
 File:    pt.h
 Popularity Tree Header File
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#ifndef PT_H_
#define PT_H_

// define and declare the data type
#include "data.h"

// Structures

typedef struct pt_node {
	data *value; ///< Data stored in the node.
	int height; ///< Height of the current node.
	int rcount; ///< Count of how many times data is retrieved.
	struct pt_node *left; ///< Pointer to the left child.
	struct pt_node *right; ///< Pointer to the right child.
} pt_node;

typedef struct pt {
	int count; ///< Number of nodes in the PT.
	pt_node *root; ///< Pointer to the root node of the PT.
	data_destroy destroy; ///< Pointer to data destroy function.
	data_copy copy; ///< Pointer to data copy function.
	data_to_string to_string; ///< Pointer to data to string function.
	data_compare compare; ///< Pointer to data comparison function.
} pt;

// Prototypes

/**
 * Allocates memory and initializes a PT structure.
 * @param destroy The destroy function for the PT data.
 * @param copy The copy function for the PT data.
 * @param to_string The to string function for the PT data.
 * @param data_compare The comparison function for the PT data.
 * @return A pointer to a new PT.
 */
pt *pt_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare);

/**
 * Deallocates memory for a PT.
 * @param tree Pointer to a PT.
 */
void pt_destroy(pt **tree);

/**
 * Determines if a PT is empty.
 * @param tree Pointer to a PT.
 * @return 1 if the PT is empty, 0 otherwise.
 */
int pt_empty(const pt *tree);

/**
 * Determines if a PT if full.
 * @param tree Pointer to a PT.
 * @return 1 if the PT if full, 0 otherwise.
 */
int pt_full(const pt *tree);

/**
 * Returns the number of elements in a PT.
 * @param tree Pointer to a PT.
 * @return The number of vales stored in the PT.
 */
int pt_count(const pt *tree);

/**
 * Inserts data into a PT.
 * @param tree Pointer to a PT. Pointer to a PT.
 * @param value Value to insert into the tree.
 * @return 1 if value is successfully inserted into the tree, 0 otherwise.
 */
int pt_insert(pt *tree, const data *value);

/**
 * Retrieves a copy of a value matching key in a PT. (Iterative)
 * @param tree Pointer to a PT.
 * @param key Key value to search for.
 * @param value The value found, if in PT.
 * @return 1 if the key is found in the PT, 0 otherwise.
 */
data *pt_retrieve(pt *tree, const data *key);

/**
 * Determines if a Popularity Tree is valid: does it meet the BST properties,
 * and are the retrieval count relationships valid.
 * @param tree Pointer to a PT.
 * @return 1 if the tree is valid, 0 otherwise.
 */
int pt_valid(const pt *tree);

/**
 * Prints the contents of the tree in preorder.
 * @param tree Pointer to a PT.
 */
void pt_preorder(const pt *tree);

#endif /* PT_H_ */
