/*
 -------------------------------------
 File:    expression_tree.c
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
#include <string.h>

#include "expression_tree.h"

// The tokenize string.
static const char SPACE[] = " \n";

/**
 * Destroys the contents of an expression tree node and its children.
 * @param node Reference pointer to an expression tree node.
 */
static void et_destroy_aux(et_node **node) {

	if (*node != NULL) {
		et_destroy_aux(&(*node)->left);
		(*node)->left = NULL;
		et_destroy_aux(&(*node)->right);
		(*node)->right = NULL;
		free((*node)->symbol);
		(*node)->symbol = NULL;
		free(*node);
		*node = NULL;
	}
	return;
}

/**
 * Builds an expression tree node. Adds an operand as a leaf or an
 * operator and its children. Assumes the expression is valid.
 * @param node Pointer to an expression tree node.
 * @param token The next token to process.
 */
static void et_build_tree_aux(et_node **node, char *token) {
	// Assignment memory to the node and its symbol.
	*node = malloc(sizeof **node);
	assert(*node != NULL);

	int n = strlen(token) + 1;
	(*node)->symbol = malloc(n);
	assert((*node)->symbol != NULL);

	strncpy((*node)->symbol, token, n);

	if (et_token_type(token) == OPERATOR) {
		// Found an operator. Append its children.
		token = strtok(NULL, SPACE);
		et_build_tree_aux(&(*node)->left, token);
		token = strtok(NULL, SPACE);
		et_build_tree_aux(&(*node)->right, token);
	} else {
		// Found an operand. Append to the tree as a leaf.
		(*node)->left = NULL;
		(*node)->right = NULL;
	}
	return;
}

/**
 *
 * @param node Pointer to an expression tree node.
 * @return The result of evaluating the current node.
 */
static double et_evaluate_aux(const et_node *node) {
	double result = 0;

	if (et_token_type(node->symbol) == OPERATOR) {
		// Found an operator.
		char c = node->symbol[0];
		double left = et_evaluate_aux(node->left);
		double right = et_evaluate_aux(node->right);

		// Determine the operation to perform.
		if (c == '+') {
			result = left + right;
		} else if (c == '-') {
			result = left - right;
		} else if (c == '*') {
			result = left * right;
		} else if (c == '/') {
			result = left / right;
		}
	} else {
		// symbol is an operand.
		result = atof(node->symbol);
	}
	return (result);
}

/**
 * Prints the node symbol and the rest of the tree in preorder.
 * @param node Pointer to an expression tree node.
 */
static char* et_preorder_aux(const et_node *node, char *str, size_t n) {

	if (node != NULL) {
		strncat(str, node->symbol, n);
		strncat(str, " ", n);
		et_preorder_aux(node->left, str, n);
		et_preorder_aux(node->right, str, n);
	}
	return (str);
}

/**
 * Prints the node symbol and the rest of the tree in inorder with
 * appropriate parentheses to show priority.
 * @param node Pointer to an expression tree node.
 */
static char* et_inorder_aux(const et_node *node, char *str, size_t n) {

	if (node != NULL) {
		et_type type = et_token_type(node->symbol);

		if (type == OPERATOR) {
			strncat(str, "( ", n);
		}
		et_inorder_aux(node->left, str, n);
		strncat(str, node->symbol, n);
		strncat(str, " ", n);
		et_inorder_aux(node->right, str, n);

		if (type == OPERATOR) {
			strncat(str, ") ", n);
		}
	}
	return (str);
}

// -------------------------------------------------------
// Functions

et_tree* et_initialize() {
	et_tree *tree = malloc(sizeof *tree);
	assert(tree != NULL);

	tree->root = NULL;
	return (tree);
}

void et_destroy(et_tree **tree) {
	et_destroy_aux(&(*tree)->root);
	free(*tree);
	*tree = NULL;
	return;
}

void et_build_tree(et_tree *tree, char *expression) {
	// Get the first token from the expression.
	char *token = strtok(expression, SPACE);
	et_build_tree_aux(&(tree->root), token);
	return;
}

char* et_preorder(const et_tree *tree, char *str, size_t n) {
	return (et_preorder_aux(tree->root, str, n));
}

char* et_inorder(const et_tree *tree, char *str, size_t n) {
	return (et_inorder_aux(tree->root, str, n));
}

et_type et_token_type(const char *token) {
	et_type type = OPERATOR;

	if (strlen(token) > 1 || strchr(OPERATORS, token[0]) == NULL) {
		type = OPERAND;
	}
	return type;
}

double et_evaluate(const et_tree *tree) {
	return (et_evaluate_aux(tree->root));
}

