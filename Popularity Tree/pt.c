/*
 -------------------------------------
 File:    pt.c
 Popularity Tree Source Code
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#include "pt.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Macro for comparing node heights
#define MAX_HEIGHT(a,b) ((a) > (b) ? a : b)
// Macros for data comparison
#define LESS_THAN_EQUAL(x,y) compare((x), (y)) <= 0
#define GREATER_THAN_EQUAL(x,y) compare((x), (y)) >= 0

// local variables
static char string[DATA_STRING_SIZE];

// Local Functions

/**
 * Initializes a new PT node with a copy of value.
 * @param tree pointer to a PT tree
 * @param value pointer to the value to assign to the node
 * @return a pointer to a new PT node
 */
static pt_node* pt_node_initialize(pt *tree, const data *value) {
	// Base case: add a new node containing a copy of value.
	pt_node *node = malloc(sizeof *node);
	assert(node != NULL);

	node->height = 1;
	node->rcount = 0;
	node->left = NULL;
	node->right = NULL;
	node->value = tree->copy(value);
	return node;
}

/**
 * Helper function to determine the height of node - handles empty node.
 * @param node The node to process.
 * @return The height of the current node.
 */
static int pt_node_height(const pt_node *node) {
	int height = 0;

	if (node != NULL) {
		height = node->height;
	}
	return height;
}

/**
 * Updates the height of a node. Its height is the max of the heights of its
 * child nodes, plus 1.
 * @param node The node to process.
 */
static void pt_update_height(pt_node *node) {
	int left_height = pt_node_height(node->left);
	int right_height = pt_node_height(node->right);

	node->height = MAX_HEIGHT(left_height, right_height) + 1;
	return;
}

/**
 * Inserts value into a PT. Insertion must preserve the PT definition.
 * Only one of value may be in the tree.
 * @param tree Pointer to a PT.
 * @param node Pointer to the node to process.
 * @param value The value to insert.
 * @return 1 if the value is inserted, 0 otherwise.
 */
static int pt_insert_aux(pt *tree, pt_node **node, const data *value) {
	int inserted = 0;

	if (*node == NULL) {
		// Base case: add a new node containing the value.
		*node = pt_node_initialize(tree, value);
		tree->count += 1;
		inserted = 1;
	} else {
		// Compare the node data against the new value.
		int comp = tree->compare((*node)->value, value);

		if (comp < 0) {
			// General case: check the left subtree.
			inserted = pt_insert_aux(tree, &(*node)->left, value);
		} else if (comp > 0) {
			// General case: check the right subtree.
			inserted = pt_insert_aux(tree, &(*node)->right, value);
		} else {
			// Base case: value is already in the PT.
			inserted = 0;
		}
	}
	if (inserted) {
		// Update the node height if any of its children have been changed.
		pt_update_height(*node);
	}
	return (inserted);
}

/**
 * Destroys a node and its children.
 * @param tree Pointer to a PT.
 * @param node The node to process.
 */
static void pt_destroy_aux(pt *tree, pt_node **node) {

	if (*node != NULL) {
		pt_destroy_aux(tree, &(*node)->left);
		pt_destroy_aux(tree, &(*node)->right);
		tree->destroy(&(*node)->value);
		(*node)->value = NULL;
		free(*node);
		*node = NULL;
	}
	return;
}

/**
 * Performs a left rotation around node.
 * @param node Pointer to the root of a subtree.
 * @return Pointer to new root of subtree.
 */
static pt_node* pt_rotate_left(pt_node *node) {
	// Rearrange the nodes.
	pt_node *temp = node->right;
	node->right = temp->left;
	temp->left = node;
	// Update the heights.
	pt_update_height(node);
	pt_update_height(temp);
	// Return new subtree root.
	return temp;
}

/**
 * Performs a right rotation around node.
 * @param node Pointer to the root of a subtree.
 * @return Pointer to new root of subtree.
 */
static pt_node* pt_rotate_right(pt_node *node) {
	// Rearrange the nodes.
	pt_node *temp = node->left;
	node->left = temp->right;
	temp->right = node;
	// Update the heights.
	pt_update_height(node);
	pt_update_height(temp);
	// Return new subtree root.
	return temp;
}

/**
 * Rebalances a node according to PT rules. Moves a node down the tree
 * if any of its children has a higher count than itself.
 * @param node Pointer to the node to rebalance.
 */
static void pt_rebalance(pt_node **node) {

	if ((*node)->left != NULL && (*node)->rcount < (*node)->left->rcount) {
		*node = pt_rotate_right(*node);
	} else if ((*node)->right != NULL && (*node)->rcount < (*node)->right->rcount) {
		*node = pt_rotate_left(*node);
	}
	return;
}

/**
 * Retrieves a key from the tree. Increments the rcount of the node
 * containing key. Rebalances the tree according to the rcount if necessary.
 * @param node The node to search for key.
 * @param key The key value to search for.
 * @param value The value to return.
 * @return 1 if key is found, 0 otherwise.
 */
static data* pt_retrieve_aux(pt *tree, pt_node **node, const data *key) {
	data *value = NULL;

	if (*node != NULL) {
		int comp = tree->compare((*node)->value, key);

		if (comp == 0) {
			// key found in tree.
			value = tree->copy((*node)->value);
			(*node)->rcount++;
		} else if (comp < 0) {
			// Search the left subtree.
			value = pt_retrieve_aux(tree, &(*node)->left, key);
		} else if (comp > 0) {
			// Search the right subtree.
			value = pt_retrieve_aux(tree, &(*node)->right, key);
		}
	}
	if (value != NULL) {
		// Rebalance the node if necessary.
		pt_rebalance(node);
	}
	return value;
}

/**
 * Determines if a Priority Tree subtree is valid.
 * @param tree Pointer to a tree
 * @param node Pointer to a PT node.
 * @return 1 if the subtree is valid, 0 otherwise.
 */
static int pt_valid_aux(const pt *tree, const pt_node *node) {
	int valid = 0;

	if (node == NULL) {
		valid = 1;
	} else if ((node->left != NULL && node->left->rcount > node->rcount)
			|| (node->right != NULL && node->right->rcount > node->rcount)) {
		// printf("Base case: retrieval count property violation.\n");
		valid = 0;
	} else if ((node->left != NULL && tree->GREATER_THAN_EQUAL(node->value, node->left->value ))
			|| (node->right != NULL && tree->LESS_THAN_EQUAL(node->value, node->right->value))) {
		// printf("Base case: node values incorrect\n");
		valid = 0;
	} else if (MAX_HEIGHT(pt_node_height(node->left),
			pt_node_height(node->right)) != (pt_node_height(node) - 1)) {
		// printf("Base case: node heights are incorrect\n");
		valid = 0;
	} else {
		valid = pt_valid_aux(tree, node->left) && pt_valid_aux(tree, node->right);
	}
	return valid;
}

/**
 * Print the contents of the subtree at node in preorder.
 * @param node Pointer to a priority tree node.
 */
static void pt_preorder_aux(const pt *tree, pt_node *node) {

	if (node != NULL) {
		printf("%s, ", tree->to_string(string, DATA_STRING_SIZE, node->value));
		pt_preorder_aux(tree, node->left);
		pt_preorder_aux(tree, node->right);
	}
}

//--------------------------------------------------------------------
// Functions

pt* pt_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare) {
	pt *tree = malloc(sizeof *tree);
	assert(tree != NULL);

	tree->root = NULL;
	tree->count = 0;
	tree->destroy = destroy;
	tree->copy = copy;
	tree->to_string = to_string;
	tree->compare = compare;
	return (tree);
}

void pt_destroy(pt **tree) {
	pt_destroy_aux(*tree, &(*tree)->root);
	free(*tree);
	*tree = NULL;
	return;
}

int pt_empty(const pt *tree) {
	return (tree->root == NULL);
}

int pt_full(const pt *tree) {
	return 0;
}

int pt_count(const pt *tree) {
	return (tree->count);
}

int pt_insert(pt *tree, const data *value) {
	return pt_insert_aux(tree, &(tree->root), value);
}

data* pt_retrieve(pt *tree, const data *key) {
	return (pt_retrieve_aux(tree, &tree->root, key));
}

int pt_valid(const pt *tree) {
	return pt_valid_aux(tree, tree->root);
}

void pt_preorder(const pt *tree) {
	pt_preorder_aux(tree, tree->root);
	printf("\n");
}
