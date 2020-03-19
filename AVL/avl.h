/*
 -------------------------------------------------------
 avl.h
 Linked version of the AVL ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#ifndef AVL_H_
#define AVL_H_

// define and declare the data type
#include "data.h"

// Structures

typedef struct avl_node {
	data *value; ///< Data stored in the node.
	int height; ///< Height of the current node.
	struct avl_node *left; ///< Pointer to the left child.
	struct avl_node *right; ///< Pointer to the right child.
} avl_node;

typedef struct avl {
	int size; ///< Number of nodes in the AVL.
	avl_node *root; ///< Pointer to the root node of the AVL.
	data_destroy destroy; ///< Pointer to data destroy function.
	data_copy copy; ///< Pointer to data copy function.
	data_to_string to_string; ///< Pointer to data to string function.
	data_compare compare; ///< Pointer to data comparison function.
} avl;

// Prototypes

/**
 * Allocates memory and initializes a AVL structure.
 * @return a pointer to the avl structure.
 */
avl* avl_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare);

/**
 * Deallocates memory for a AVL.
 * @param tree Pointer to a AVL.
 */
void avl_destroy(avl **tree);

/**
 * Determines if a AVL is empty.
 * @param tree Pointer to a AVL.
 * @return 1 if the AVL is empty, 0 otherwise.
 */
int avl_empty(const avl *tree);

/**
 * Determines if a AVL if full.
 * @param tree Pointer to a AVL.
 * @return 1 if the AVL if full, 0 otherwise.
 */
int avl_full(const avl *tree);

/**
 * Returns the number of elements in a AVL.
 * @param tree Pointer to a AVL.
 * @return The number of vales stored in the AVL.
 */
int avl_size(const avl *tree);

/**
 * Inserts data into a AVL.
 * @param tree Pointer to a AVL. Pointer to a AVL.
 * @param value Value to insert into the tree.
 * @return 1 if value is successfully inserted into the tree, 0 otherwise.
 */
int avl_insert(avl *tree, const data *value);

/**
 * Retrieves a copy of a value matching key in a AVL. (Iterative)
 * @param tree Pointer to a AVL.
 * @param key Key value to search for.
 * @param value The value found, if in AVL.
 * @return copy of data if the key is found in the tree, NULL otherwise.
 */
data* avl_retrieve(const avl *tree, const data *key);

/**
 * Removes a node with a value matching key from the avl.
 * @param tree Pointer to a AVL.
 * @param key Key value to search for.
 * @param value The value found, if in AVL.
 * @return pointer to data if the key is found in the tree, NULL otherwise.
 */
data* avl_remove(avl *tree, const data *key);

/**
 * Copies the contents of a tree to an array in inorder.
 * @param tree Pointer to a tree.
 * @param values list of data. Length must be size of tree.
 */
void avl_inorder(const avl *tree, data *values);

/**
 * Copies the contents of a tree to an array in preorder.
 * @param tree Pointer to a tree.
 * @param values list of data. Length must be size of tree.
 */
void avl_preorder(const avl *tree, data *values);

/**
 * Copies the contents of a tree to an array in postorder.
 * @param tree Pointer to a tree.
 * @param values list of data. Length must be size of tree.
 */
void avl_postorder(const avl *tree, data *values);

/**
 * Find the maximum value in the tree.
 * @param tree Pointer to a tree.
 * @return Maximum value in tree.
 */
data* avl_max(const avl *tree);

/**
 * Returns a copy of the minimum value in the tree.
 * @param tree Pointer to an tree.
 * @return Copy of minimum value in tree.
 */
data* avl_min(const avl *tree);

/**
 * Finds the number of leaf nodes in a tree.
 * @param tree Pointer to a AVL.
 * @return Number of nodes with no children.
 */
int avl_leaf_count(const avl *tree);

/**
 * Finds the number of nodes with one child in a tree.
 * @param tree Pointer to a AVL.
 * @return Number of nodes with one child.
 */
int avl_one_child_count(const avl *tree);

/**
 * Finds the number of nodes with two children in a tree.
 * @param tree Pointer to a AVL.
 * @return Number of nodes with two children.
 */
int avl_two_child_count(const avl *tree);

/**
 * Determines the number of nodes with zero, one, and two children.
 * @param tree Pointer to a AVL.
 * @param zero Number of leaf nodes (no children).
 * @param one Number of nodes with one child.
 * @param two Number of nodes with two children.
 */
void avl_node_counts(const avl *tree, int *zero, int *one, int *two);

/**
 * Determines whether or not a tree is a balanced tree.
 * All node heights are no more than one greater than any child heights.
 * @param tree Pointer to a AVL.
 * @return
 */
int avl_balanced(const avl *tree);

/**
 * Determines whether or not a tree is a valid AVL.
 * @param tree Pointer to a AVL.
 * @return 1 if the tree is a valid AVL, 0 otherwise.
 */
int avl_valid(const avl *tree);

/**
 * Determines if two trees contain same data in same configuration.
 * @param target Pointer to an tree.
 * @param source Pointer to an tree.
 * @return 1 if trees are identical, 0 otherwise.
 */
int avl_equals(const avl *target, const avl *source);

#endif /* AVL_H_ */
