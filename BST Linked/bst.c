/*
 -------------------------------------------------------
 bst.c
 Linked version of the BST ADT.
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */
#include "bst.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Macro for comparing node heights
#define MAX_HEIGHT(a,b) ((a) > (b) ? a : b)

// local variables
static char string[DATA_STRING_SIZE];

// local functions

/**
 * Initializes a new BST node with a copy of value.
 * @param tree pointer to a BST tree
 * @param value pointer to the value to assign to the node
 * @return a pointer to a new BST node
 */
static bst_node* bst_node_initialize(bst *tree, const data *value) {
	// Base case: add a new node containing a copy of value.
	bst_node *node = malloc(sizeof *node);
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
static int bst_node_height(const bst_node *node) {
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
static void bst_update_height(bst_node *node) {
	int left_height = bst_node_height(node->left);
	int right_height = bst_node_height(node->right);

	node->height = MAX_HEIGHT(left_height, right_height) + 1;
	return;
}

/**
 * Destroys a node and its children.
 * @param tree Pointer to a BST.
 * @param node Pointer to the node to destroy.
 */
static void bst_destroy_aux(bst *tree, bst_node **node) {

	if (*node != NULL) {
		bst_destroy_aux(tree, &(*node)->left);
		bst_destroy_aux(tree, &(*node)->right);
		tree->destroy(&(*node)->value);
		(*node)->value = NULL;
		free(*node);
		*node = NULL;
	}
	return;
}

/**
 * Inserts value into a BST. Insertion must preserve the BST definition.
 * Only one of value may be in the tree.
 * @param tree Pointer to a BST.
 * @param node Pointer to the node to process.
 * @param value The value to insert.
 * @return 1 if the value is inserted, 0 otherwise.
 */
static int bst_insert_aux(bst *tree, bst_node **node, const data *value) {
	int inserted = 0;

	if (*node == NULL) {
		// Base case: add a new node containing the value.
		*node = bst_node_initialize(tree, value);
		tree->count += 1;
		inserted = 1;
	} else {
		// Compare the node data against the new value.
		int comp = tree->compare((*node)->value, value);

		if (comp < 0) {
			// General case: check the left subtree.
			inserted = bst_insert_aux(tree, &(*node)->left, value);
		} else if (comp > 0) {
			// General case: check the right subtree.
			inserted = bst_insert_aux(tree, &(*node)->right, value);
		} else {
			// Base case: value is already in the BST.
			inserted = 0;
		}
	}
	if (inserted) {
		// Update the node height if any of its children have been changed.
		bst_update_height(*node);
	}
	return inserted;
}

/**
 * Finds a replacement node for a node to be removed from the tree.
 * (Called only by bst_remove_aux.)
 * @param parent Parent of possible replacment node.
 * @return The node to replace the node that is being removed.
 */
static bst_node* bst_delete_node(bst_node *parent) {
	bst_node *repl_node = NULL;
	bst_node *child = parent->right;

	if (child->right == NULL) {
		// child has largest value in subtree and is the replacement node
		repl_node = child;
		// move child's left tree up to right of parent
		parent->right = child->left;
	} else {
		// Continue search for node with largest value
		repl_node = bst_delete_node(child);
	}
	// Recursively update all parent node heights
	bst_update_height(parent);
	return repl_node;
}

/**
 * Attempts to find a value matching key in a BST node. Deletes the node
 * if found.
 * @param tree Pointer to a BST.
 * @param node The node to process.
 * @param key The key to look for.
 * @return data if the key is found and the value removed, NULL otherwise.
 */
static data* bst_remove_aux(bst *tree, bst_node **node, const data *key) {
	bst_node *repl_node = NULL;
	data *value = NULL;

	if (*node != NULL) {
		// Compare the node data against the key.
		int comp = tree->compare((*node)->value, key);

		if (comp < 0) {
			// Search the left subtree.
			value = bst_remove_aux(tree, &((*node)->left), key);
		} else if (comp > 0) {
			// Search the right subtree.
			value = bst_remove_aux(tree, &((*node)->right), key);
		} else {
			// Value has been found.
			value = (*node)->value;
			tree->count--;
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
					// left child of replaced node is replacement node
					repl_node = (*node)->left;
				} else {
					// Find replacement node in right subtree of left child
					repl_node = bst_delete_node((*node)->left);
					// Make the left child of the node being replaced the left child
					// of the replacement node.
					repl_node->left = (*node)->left;
				}
				// Make the right child of the node being replaced the right child
				// of the replacement node.
				repl_node->right = (*node)->right;
				// Replace the removed node.
				*node = repl_node;
			}
		}
	}
	if (*node != NULL && value != NULL) {
		// If the value was found, update the ancestor heights.
		bst_update_height(*node);
	}
	return value;
}

/**
 * Prints the contents of the BST in inorder.
 * @param tree Pointer to a BST.
 * @param node The node to process.
 */
static void bst_inorder_aux(const bst *tree, const bst_node *node) {

	if (node != NULL) {
		bst_inorder_aux(tree, node->left);
		printf("%s\n", tree->to_string(string, DATA_STRING_SIZE, node->value));
		bst_inorder_aux(tree, node->right);
	}
	return;
}

/**
 * Prints the contents of the tree in preorder.
 * @param tree Pointer to a BST.
 * @param node The node to process.
 */
static void bst_preorder_aux(const bst *tree, const bst_node *node) {

	if (node != NULL) {
		printf("%s\n", tree->to_string(string, DATA_STRING_SIZE, node->value));
		bst_preorder_aux(tree, node->left);
		bst_preorder_aux(tree, node->right);
	}
	return;
}

static void bst_postorder_aux(const bst *tree, const bst_node *node) {

	if (node != NULL) {
		bst_postorder_aux(tree, node->left);
		bst_postorder_aux(tree, node->right);
		printf("%s\n", tree->to_string(string, DATA_STRING_SIZE, node->value));
	}
	return;
}

/**
 * Returns the number of leaves (nodes with no children) in node.
 * @param node The node to process.
 * @return The number of leaves for node and its children.
 */
static int bst_leaf_count_aux(const bst_node *node) {
	int count = 0;

	if (node == NULL) {
		// Base case: no node.
		count = 0;
	} else if (node->left == NULL && node->right == NULL) {
		// Base case: node has no children.
		count = 1;
	} else {
		count = bst_leaf_count_aux(node->left)
				+ bst_leaf_count_aux(node->right);
	}
	return count;
}

/**
 * Returns the number of one child nodes in a BST node.
 * @param node The node to process.
 * @return The number of one child nodes for node and its children.
 */
static int bst_one_child_count_aux(const bst_node *node) {
	int count = 0;

	if (node == NULL) {
		// Base case: empty node..
		count = 0;
	} else if (node->left == NULL && node->right != NULL) {
		// General case: node has one child.
		count = 1 + bst_one_child_count_aux(node->right);
	} else if (node->left != NULL && node->right == NULL) {
		// General case: node has one child.
		count = 1 + bst_one_child_count_aux(node->left);
	} else {
		// General case: node has two children.
		count = bst_one_child_count_aux(node->left)
				+ bst_one_child_count_aux(node->right);
	}
	return count;
}

/**
 * Returns the number of types of nodes in a BST node.
 * @param node The node to process.
 * @return The number of two children nodes for node and its children.
 */
static int bst_two_child_count_aux(bst_node *node) {
	int count = 0;

	if (node == NULL) {
		// Base case: no node.
		count = 0;
	} else if (node->left != NULL && node->right != NULL) {
		// General case: node has two children.
		count = 1 + bst_two_child_count_aux(node->left)
				+ bst_two_child_count_aux(node->right);
	} else {
		// General case: node has one child.
		count = bst_two_child_count_aux(node->left)
				+ bst_two_child_count_aux(node->right);
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
static void bst_node_counts_aux(bst_node *node, int *zero, int *one, int *two) {

	if (node != NULL) {

		if (node->left == NULL && node->right == NULL) {
			// Base case: leaf node
			(*zero)++;
		} else if (node->left == NULL && node->right != NULL) {
			// One child - right
			(*one)++;
			bst_node_counts_aux(node->right, zero, one, two);
		} else if (node->left != NULL && node->right == NULL) {
			// One child - left
			(*one)++;
			bst_node_counts_aux(node->left, zero, one, two);
		} else {
			// two children
			(*two)++;
			bst_node_counts_aux(node->left, zero, one, two);
			bst_node_counts_aux(node->right, zero, one, two);
		}
	}
	return;
}

/**
 * Determines whether the BST is balanced.
 * @param node The node to process.
 * @return 1 if the node and its children are balanced, 0 otherwise.
 */
static int bst_balanced_aux(const bst_node *node) {
	int balanced = 0;

	if (node == NULL || node->height == 1) {
		// Base case: no node or a leaf, so no children.
		balanced = 1;
	} else if (abs(bst_node_height(node->left) - bst_node_height(node->right))
			> 1) {
		// Base case: left or right subtree is too deep.
		balanced = 0;
	} else {
		// General case: check the children of node.
		balanced = bst_balanced_aux(node->left)
				&& bst_balanced_aux(node->right);
	}
	return balanced;
}

/**
 * Determines if a tree is a valid BST.
 * @param tree Pointer to a BST.
 * @param node The node to process.
 * @return 1 if the node and its children are valid, 0 otherwise.
 */
static int bst_valid_aux(const bst *tree, const bst_node *node,
		const bst_node *min_node, const bst_node *max_node) {
	int valid = 0;

	if (node == NULL) {
		// Base case: no node
		valid = 1;
	} else if (min_node != NULL
			&& tree->compare(min_node->value, node->value) <= 0) {
		// Base case: node value less = than min_node value
		valid = 0;
	} else if (max_node != NULL
			&& tree->compare(max_node->value, node->value) >= 0) {
		// Base case: node value greater = max_node value
		valid = 0;
	} else if (MAX_HEIGHT(bst_node_height(node->left),
			bst_node_height(node->right)) > (bst_node_height(node) + 1)) {
		// Base case: node heights are incorrect
		valid = 0;
	} else {
		valid = bst_valid_aux(tree, node->left, min_node, node)
				&& bst_valid_aux(tree, node->right, node, max_node);
	}
	return valid;
}

//--------------------------------------------------------------------
// Functions

bst* bst_initialize(data_destroy destroy, data_copy copy,
		data_to_string to_string, data_compare compare) {
	bst *tree = malloc(sizeof *tree);
	assert(tree != NULL);

	tree->root = NULL;
	tree->count = 0;
	tree->destroy = destroy;
	tree->copy = copy;
	tree->to_string = to_string;
	tree->compare = compare;
	return tree;
}

void bst_destroy(bst **tree) {
	bst_destroy_aux(*tree, &(*tree)->root);
	free(*tree);
	*tree = NULL;
	return;
}

int bst_empty(const bst *tree) {
	return (tree->root == NULL);
}

int bst_full(const bst *tree) {
	return 0;
}

int bst_count(const bst *tree) {
	return tree->count;
}

void bst_inorder(const bst *tree) {
	bst_inorder_aux(tree, tree->root);
	printf("\n");
	return;
}

void bst_preorder(const bst *tree) {
	bst_preorder_aux(tree, tree->root);
	printf("\n");
	return;
}

void bst_postorder(const bst *tree) {
	bst_postorder_aux(tree, tree->root);
	printf("\n");
	return;
}

int bst_insert(bst *tree, const data *value) {
	return bst_insert_aux(tree, &(tree->root), value);
}

data* bst_retrieve(const bst *tree, const data *key) {
	bst_node *node = tree->root;
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

data* bst_remove(bst *tree, const data *key) {
	return bst_remove_aux(tree, &(tree->root), key);
}

data* bst_max(const bst *tree) {
	assert(tree->root != NULL);

	// Find the node containing the largest data.
	// (It is the right-most node.)
	bst_node *node = tree->root;

	while (node->right != NULL) {
		node = node->right;
	}
	return tree->copy(node->value);
}

data* bst_min(const bst *tree) {
	assert(tree->root != NULL);

	// Find the node containing the largest data.
	// (It is the right-most node.)
	bst_node *node = tree->root;

	while (node->left != NULL) {
		node = node->left;
	}
	return tree->copy(node->value);
}

void bst_node_counts(const bst *tree, int *zero, int *one, int *two) {
	bst_node_counts_aux(tree->root, zero, one, two);
	return;
}

int bst_leaf_count(const bst *tree) {
	return bst_leaf_count_aux(tree->root);
}

int bst_one_child_count(const bst *tree) {
	return bst_one_child_count_aux(tree->root);
}

int bst_two_child_count(const bst *tree) {
	return bst_two_child_count_aux(tree->root);
}

int bst_balanced(const bst *tree) {
	return bst_balanced_aux(tree->root);
}

int bst_valid(const bst *tree) {
	return bst_valid_aux(tree, tree->root, NULL, NULL);
}
