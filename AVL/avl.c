/*
 -------------------------------------------------------
 avl.c
 Linked version of the AVL ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#include "avl.h"

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Macro for comparing node heights
#define MAX_HEIGHT(a,b) ((a) > (b) ? a : b)
// Macros for data comparison
#define LESS_THAN_EQUAL(x,y) compare((x), (y)) <= 0
#define GREATER_THAN_EQUAL(x,y) compare((x), (y)) >= 0

// Local Functions

/**
 * Initializes a new AVL node with a copy of value.
 * @param tree pointer to a AVL tree
 * @param value pointer to the value to assign to the node
 * @return a pointer to a new AVL node
 */
static avl_node* avl_node_initialize(avl *tree, const data *value) {
	// Base case: add a new node containing a copy of value.
	avl_node *node = malloc(sizeof *node);
	assert(node != NULL);

	node->height = 1;
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
static int avl_node_height(const avl_node *node) {
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
static void avl_update_height(avl_node *node) {
	int left_height = avl_node_height(node->left);
	int right_height = avl_node_height(node->right);

	node->height = MAX_HEIGHT(left_height, right_height) + 1;
	return;
}

/**
 * Destroys a node and its children.
 * @param tree Pointer to a AVL.
 * @param node The node to process.
 */
static void avl_destroy_aux(avl *tree, avl_node **node) {

	if (*node != NULL) {
		avl_destroy_aux(tree, &(*node)->left);
		avl_destroy_aux(tree, &(*node)->right);
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
static avl_node* avl_rotate_left(avl_node *node) {
	// Rearrange the nodes.
	avl_node *temp = node->right;
	node->right = temp->left;
	temp->left = node;
	// Update the heights.
	avl_update_height(node);
	avl_update_height(temp);
	// Return new root.
	return temp;
}

/**
 * Performs a right rotation around node.
 * @param node Pointer to the root of a subtree.
 * @return Pointer to new root of subtree.
 */
static avl_node* avl_rotate_right(avl_node *node) {
	// Rearrange the nodes.
	avl_node *temp = node->left;
	node->left = temp->right;
	temp->right = node;
	// Update the heights.
	avl_update_height(node);
	avl_update_height(temp);
	// Return new root.
	return temp;
}

/**
 * Determines whether an AVL node is balanced.
 * @param node The node to process.
 * @return an integer between -2 (right-heavy) and 2 (left-heavy)
 */
static int avl_balance_value(const avl_node *node) {
	return avl_node_height(node->left) - avl_node_height(node->right);
}

/**
 * Rebalances a node according to AVL rules.
 * @param node Pointer to the node to rebalance.
 */
static void avl_rebalance(avl_node **node) {
	// Update the node height if any of its children have been changed.
	avl_update_height(*node);
	// Get the balance factor of this ancestor node to check whether
	// this node became unbalanced
	int balance = avl_balance_value(*node);
	// If this node is unbalanced, then there are 4 cases

	if (balance > 1 && avl_balance_value((*node)->left) >= 0) {
		// Left Left Case - single rotation
		*node = avl_rotate_right(*node);
	} else if (balance < -1 && avl_balance_value((*node)->right) <= 0) {
		// Right Right Case - single rotation
		*node = avl_rotate_left(*node);
	} else if (balance > 1 && avl_balance_value((*node)->left) < 0) {
		// Left Right Case - double rotation
		(*node)->left = avl_rotate_left((*node)->left);
		*node = avl_rotate_right(*node);
	} else if (balance < -1 && avl_balance_value((*node)->right) > 0) {
		// Right Left Case - double rotation
		(*node)->right = avl_rotate_right((*node)->right);
		*node = avl_rotate_left(*node);
	}
	return;
}

/**
 * Inserts value into a AVL. Insertion must preserve the AVL definition.
 * Only one of value may be in the tree.
 * @param tree Pointer to a AVL.
 * @param node The node to process.
 * @param value The value to insert.
 * @return 1 if the value is inserted, 0 otherwise.
 */
static int avl_insert_aux(avl *tree, avl_node **node, const data *value) {
	int inserted = 0;

	if (*node == NULL) {
		// Base case: add a new node containing the value.
		*node = avl_node_initialize(tree, value);
		tree->size += 1;
		inserted = 1;
	} else {
		// Compare the node data against the new value.
		int comp = tree->compare((*node)->value, value);

		if (comp < 0) {
			// General case: check the left subtree.
			inserted = avl_insert_aux(tree, &(*node)->left, value);
		} else if (comp > 0) {
			// General case: check the right subtree.
			inserted = avl_insert_aux(tree, &(*node)->right, value);
		} else {
			// Base case: value is already in the AVL.
			inserted = 0;
		}
	}
	if (inserted) {
		// Rebalance if any of its children have been changed.
		avl_rebalance(node);
	}
	return inserted;
}

/**
 * Finds a replacement node for a node to be removed from the tree.
 * (Called only by avl_remove_aux.)
 * @param parent Parent of possible replacement node.
 * @return The node to replace the node that is being removed.
 */
static avl_node* avl_delete_node_left(avl_node **parent) {
	avl_node *repl = NULL;
	avl_node *child = (*parent)->right;

	if (child->right == NULL) {
		// child has largest value in left subtree.
		repl = child;
		// Move child's left tree up.
		(*parent)->right = child->left;
	} else {
		repl = avl_delete_node_left(&(*parent)->right);
	}
	// Recursively rebalance all parents as necessary.
	avl_rebalance(parent);
	return repl;
}

/**
 * Attempts to find a value matching key in a AVL node. Deletes the node
 * if found.
 * @param tree Pointer to a AVL.
 * @param node The node to process.
 * @param key The key to look for.
 * @param value If key is found, the value being removed.
 * @return 1 if the key is found and the value removed, 0 otherwise.
 */
static data* avl_remove_aux(avl *tree, avl_node **node, const data *key) {
	avl_node *repl = NULL;
	data *value = NULL;

	if (*node != NULL) {
		// Compare the node data against the key.
		int comp = tree->compare((*node)->value, key);

		if (comp < 0) {
			// Search the left subtree.
			value = avl_remove_aux(tree, &((*node)->left), key);
		} else if (comp > 0) {
			// Search the right subtree.
			value = avl_remove_aux(tree, &((*node)->right), key);
		} else {
			// Value has been found.
			value = (*node)->value;
			tree->size--;

			// Replace this node with another node.
			if ((*node)->left == NULL && (*node)->right == NULL) {
				// node has no children.
				*node = NULL;
			} else if ((*node)->left == NULL) {
				// node has no left child.
				*node = (*node)->right;
			} else if ((*node)->right == NULL) {
				// node has no right child.
				*node = (*node)->left;
			} else {
				// Node has two children
				if ((*node)->left->right == NULL) {
					// left child is replacement node
					repl = (*node)->left;
				} else {
					// find replacement node in right subtree of left node
					repl = avl_delete_node_left(&(*node)->left);
					// Make the left child of the node being replaced the left child
					// of the replacement node.
					repl->left = (*node)->left;
				}
				// Make the right child of the node being replaced the right child
				// of the replacement node.
				repl->right = (*node)->right;
				// Replace the removed node.
				*node = repl;
			}
		}
	}
	if (*node != NULL && value != NULL) {
		// If the value was found, update the ancestor heights.
		avl_rebalance(node);
	}
	return value;
}

/**
 * Copies the contents of a node to an array location.
 * @param tree Pointer to a tree.
 * @param node Pointer to node.
 * @param values Array of values.
 * @param index Current index in array.
 * @return the updated index.
 */
static int avl_inorder_aux(const avl *tree, const avl_node *node, data values[],
		int index) {

	if (node != NULL) {
		index = avl_inorder_aux(tree, node->left, values, index);
		values[index] = *node->value;
		index++;
		index = avl_inorder_aux(tree, node->right, values, index);
	}
	return index;
}

/**
 * Copies the contents of a node to an array location.
 * @param tree Pointer to a tree.
 * @param node Pointer to node.
 * @param values Array of values.
 * @param index Current index in array.
 * @return the updated index.
 */
static int avl_preorder_aux(const avl *tree, const avl_node *node,
		data values[], int index) {

	if (node != NULL) {
		values[index] = *(node->value);
		index++;
		index = avl_preorder_aux(tree, node->left, values, index);
		index = avl_preorder_aux(tree, node->right, values, index);
	}
	return index;
}

/**
 * Copies the contents of a node to an array location.
 * @param tree Pointer to a tree.
 * @param node Pointer to node.
 * @param values Array of values.
 * @param index Current index in array.
 * @return the updated index.
 */
static int avl_postorder_aux(const avl *tree, const avl_node *node,
		data values[], int index) {

	if (node != NULL) {
		index = avl_postorder_aux(tree, node->left, values, index);
		index = avl_postorder_aux(tree, node->right, values, index);
		values[index] = *(node->value);
		index++;
	}
	return index;
}
/**
 * Returns the number of leaves (nodes with no children) in node.
 * @param node The node to process.
 * @return The number of leaves for node and its children.
 */
static int avl_leaf_count_aux(const avl_node *node) {
	int count = 0;

	if (node == NULL) {
		// Base case: no node.
		count = 0;
	} else if (node->left == NULL && node->right == NULL) {
		// Base case: node has no children.
		count = 1;
	} else {
		count = avl_leaf_count_aux(node->left)
				+ avl_leaf_count_aux(node->right);
	}
	return count;
}

/**
 * Returns the number of one child nodes in a AVL node.
 * @param node The node to process.
 * @return The number of one child nodes for node and its children.
 */
static int avl_one_child_count_aux(const avl_node *node) {
	int count = 0;

	if (node == NULL) {
		// Base case: empty node..
		count = 0;
	} else if (node->left == NULL && node->right != NULL) {
		// General case: node has one child.
		count = 1 + avl_one_child_count_aux(node->right);
	} else if (node->left != NULL && node->right == NULL) {
		// General case: node has one child.
		count = 1 + avl_one_child_count_aux(node->left);
	} else {
		// General case: node has two children.
		count = avl_one_child_count_aux(node->left)
				+ avl_one_child_count_aux(node->right);
	}
	return count;
}

/**
 * Returns the number of types of nodes in a AVL node.
 * @param node The node to process.
 * @return The number of two children nodes for node and its children.
 */
static int avl_two_child_count_aux(const avl_node *node) {
	int count = 0;

	if (node == NULL) {
		// Base case: no node.
		count = 0;
	} else if (node->left != NULL && node->right != NULL) {
		// General case: node has two children.
		count = 1 + avl_two_child_count_aux(node->left)
				+ avl_two_child_count_aux(node->right);
	} else {
		// General case: node has one child.
		count = avl_two_child_count_aux(node->left)
				+ avl_two_child_count_aux(node->right);
	}
	return count;
}

/**
 * Determines the number of zero, one, and two children nodes in a tree.
 * @param node The node to process.
 * @param zero The number of zero children nodes for node and its children.
 * @param one The number of one child nodes for node and its children.
 * @param two The number of two children nodes for node and its children.
 */
static void avl_node_counts_aux(const avl_node *node, int *zero, int *one,
		int *two) {

	if (node != NULL) {

		if (node->left == NULL && node->right == NULL) {
			// Base case: leaf node
			(*zero)++;
		} else if (node->left == NULL && node->right != NULL) {
			// One child - right
			(*one)++;
			avl_node_counts_aux(node->right, zero, one, two);
		} else if (node->left != NULL && node->right == NULL) {
			// One child - left
			(*one)++;
			avl_node_counts_aux(node->left, zero, one, two);
		} else {
			// two children
			(*two)++;
			avl_node_counts_aux(node->left, zero, one, two);
			avl_node_counts_aux(node->right, zero, one, two);
		}
	}
	return;
}

/**
 * Determines whether the AVL is balanced.
 * @param node The node to process.
 * @return 1 if the node and its children are balanced, 0 otherwise.
 */
static int avl_balanced_aux(const avl_node *node) {
	int balanced = 0;

	if (node == NULL || node->height == 1) {
		// Base case: no node or a leaf, so no children.
		balanced = 1;
	} else if (abs(avl_node_height(node->left) - avl_node_height(node->right))
			> 1) {
		// Base case: left or right subtree is too deep.
		balanced = 0;
	} else {
		// General case: check the children of node.
		balanced = avl_balanced_aux(node->left)
				&& avl_balanced_aux(node->right);
	}
	return balanced;
}

/**
 * Determines if a tree is a valid AVL.
 * @param tree pointer to a tree.
 * @param node The node to process.
 * @return 1 if the node and its children are valid, 0 otherwise.
 */
static int avl_valid_aux(const avl *tree, const avl_node *node,
		const avl_node *min_node, const avl_node *max_node) {
	int valid = 0;

	if (node == NULL) {
		// Base case: no node
		valid = 1;
	} else if (min_node != NULL
			&& tree->LESS_THAN_EQUAL(min_node->value, node->value)) {
		// Base case: node value less = than min_node value
		valid = 0;
	} else if (max_node != NULL
			&& tree->GREATER_THAN_EQUAL(max_node->value, node->value)) {
		// Base case: node value greater = max_node value
		valid = 0;
	} else if (abs(avl_node_height(node->left) - avl_node_height(node->right))
			> 1) {
		// Base case: height violation - child heights not balanced
		valid = 0;
	} else if (MAX_HEIGHT(avl_node_height(node->left),
			avl_node_height(node->right)) != (node->height - 1)) {
		// Base case: node heights are incorrect
		valid = 0;
	} else {
		valid = avl_valid_aux(tree, node->left, min_node, node)
				&& avl_valid_aux(tree, node->right, node, max_node);
	}
	return valid;
}

/**
 * Determines if two trees are identical.
 * @param node1 Pointer to an AVL node.
 * @param node2 Pointer to an AVL node.
 * @return 1 if trees are identical, 0 otherwise.
 */
static int avl_equals_aux(const avl *tree, const avl_node *target,
		const avl_node *source) {
	int equals = 0;

	if (target == NULL && source == NULL) {
		// Base case: Trees are identical so far.
		equals = 1;
	} else if ((target == NULL && source != NULL)
			|| (target != NULL && source == NULL)
			|| tree->compare(target->value, source->value) == 0) {
		// Base case: tree elements are not equal.
		equals = 0;
	} else {
		// General case: search rest of trees.
		equals = avl_equals_aux(tree, target->left, source->left)
				&& avl_equals_aux(tree, target->right, source->right);
	}
	return equals;
}

//--------------------------------------------------------------------
// Functions

avl* avl_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare) {
	avl *tree = malloc(sizeof *tree);
	assert(tree != NULL);

	tree->root = NULL;
	tree->size = 0;
	tree->destroy = destroy;
	tree->copy = copy;
	tree->to_string = to_string;
	tree->compare = compare;
	return tree;
}

void avl_destroy(avl **tree) {
	avl_destroy_aux(*tree, &(*tree)->root);
	free(*tree);
	*tree = NULL;
	return;
}

int avl_empty(const avl *tree) {
	return tree->root == NULL;
}

int avl_full(const avl *tree) {
	return 0;
}

int avl_size(const avl *tree) {
	return tree->size;
}

void avl_inorder(const avl *tree, data *values) {
	avl_inorder_aux(tree, tree->root, values, 0);
	return;
}

void avl_preorder(const avl *tree, data *values) {
	avl_preorder_aux(tree, tree->root, values, 0);
	return;
}

void avl_postorder(const avl *tree, data *values) {
	avl_postorder_aux(tree, tree->root, values, 0);
	return;
}

int avl_insert(avl *tree, const data *value) {
	return avl_insert_aux(tree, &(tree->root), value);
}

data* avl_retrieve(const avl *tree, const data *key) {
	avl_node *node = tree->root;
	data *value = NULL;

	while (node != NULL && value == NULL) {
		int comp = tree->compare(node->value, key);

		if (comp < 0) {
			node = node->left;
		} else if (comp > 0) {
			node = node->right;
		} else {
			value = tree->copy(node->value);
		}
	}
	return value;
}

data* avl_remove(avl *tree, const data *key) {
	return avl_remove_aux(tree, &(tree->root), key);
}

data* avl_max(const avl *tree) {
	assert(tree->root != NULL);

	// Find the node containing the largest data.
	// (It is the right-most node.)
	avl_node *node = tree->root;

	while (node->right != NULL) {
		node = node->right;
	}
	return tree->copy(node->value);
}

data* avl_min(const avl *tree) {
	assert(tree->root != NULL);

	// Find the node containing the largest data.
	// (It is the right-most node.)
	avl_node *node = tree->root;

	while (node->left != NULL) {
		node = node->left;
	}
	return tree->copy(node->value);
}

void avl_node_counts(const avl *tree, int *zero, int *one, int *two) {
	*zero = *one = *two = 0;
	avl_node_counts_aux(tree->root, zero, one, two);
	return;
}

int avl_leaf_count(const avl *tree) {
	return avl_leaf_count_aux(tree->root);
}

int avl_one_child_count(const avl *tree) {
	return avl_one_child_count_aux(tree->root);
}

int avl_two_child_count(const avl *tree) {
	return avl_two_child_count_aux(tree->root);
}

int avl_balanced(const avl *tree) {
	return avl_balanced_aux(tree->root);
}

int avl_valid(const avl *tree) {
	return avl_valid_aux(tree, tree->root, NULL, NULL);
}

int avl_equals(const avl *target, const avl *source) {
	return avl_equals_aux(source, target->root, source->root);
}
