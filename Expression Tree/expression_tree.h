/*
 -------------------------------------
 File:    expression_tree.h
 -------------------------------------------------------
 Author:       Laksitha Dissanayake
 ID:           170870810
 Email:        diss0810@wlu.ca
 Version:      2019-05-27
 -------------------------------------------------------
 */

#ifndef EXPRESSION_TREE_H_
#define EXPRESSION_TREE_H_

// String of allowed operators.
#define OPERATORS "+-*/"

/**
 * Defines a symbol as either an operator (as above) or an operand.
 */
typedef enum {
	OPERATOR, OPERAND
} et_type;

/**
 * Defines the structure of an expression tree node.
 */
typedef struct et_node {
	char *symbol; ///< String representation of an operator or operand.
	struct et_node *left; ///< Pointer to the left child node.
	struct et_node *right; ///< Pointer to the right child node.
} et_node;

/**
 * Defines the structure of an expression tree.
 */
typedef struct {
	et_node *root; ///< Pointer to the root node of an expression tree.
} et_tree;

/**
 * Initializes an expression tree.
 * @return Pointer to an expression tree.
 */
et_tree *et_initialize();

/**
 * Destroys the contents of an expression tree.
 * @param tree Pointer to an expression tree.
 */
void et_destroy(et_tree **tree);

/**
 * Copies the contents of an expression tree in preorder to a string.
 * @param tree Pointer to an expression tree.
 * @param str String to store result.
 * @param n Size of str buffer.
 * @return Pointer to str.
 */
char *et_preorder(const et_tree *tree, char *str, size_t n);

/**
 * Copies the contents of an expression tree in inorder, with parentheses, to a string.
 * @param tree Pointer to an expression tree.
 * @param str String to store result.
 * @param n Size of str buffer.
 * @return Pointer to str.
 */
char *et_inorder(const et_tree *tree, char *str, size_t n);

/**
 * Builds an expression tree given a valid prefix expression.
 * @param tree Pointer to an expression tree.
 * @param expression The string expression to process.
 */
void et_build_tree(et_tree *tree, char *expression);

/**
 * Determines the type (operator or operand) of a symbol.
 * @param token The string token to determine the type of.
 * @return OPERATOR if token is an operator, OPERAND otherwise.
 */
et_type et_token_type(const char *token);

/**
 * Evaluates the expression stored in the expression tree.
 * @param tree Pointer to an expression tree.
 * @return The evaluation of the expression tree.
 */
double et_evaluate(const et_tree *tree);

#endif /* EXPRESSION_TREE_H_ */
