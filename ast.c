#include <stdio.h>
#include "ast.h"
#include "token.h"

// ORDER OF OPERATIONS:
// 1. Parentheses
// 2. Exponents
// 3. Multiplication/Division
// 4. Addition/Subtraction
static struct node *parse_primary(const struct list *tokens, size_t *index);

static struct node *parse_multiply(const struct list *tokens, size_t *index);

static struct node *parse_add(const struct list *tokens, size_t *index);

static struct node *build_ast(const struct list *tokens, size_t *index);

bool is_unary_op(enum token_type type) {
    return type == U_MINUS;
}

bool is_binary_op(enum token_type type) {
    return type >= PLUS && type <= POWER;
}

struct node *create_node(struct token *token) {
    struct node *node = malloc(sizeof *node);
    if (node == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    node->token = token->type;
    node->val = token->data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static struct node *parse_primary(const struct list *tokens, size_t *index) {
    struct token *current = list_get(tokens, *index);
    switch (current->type) {
        case NUMBER: {
            (*index)++;
            struct node *new_node = create_node(current);
            new_node->node_type = LEAF;
            return new_node;
        }
        case L_PAREN: {
            (*index)++;
            struct node *new_node = build_ast(tokens, index);
            if (((struct token *) list_get(tokens, *index))->type != R_PAREN) {
                puts("Expected ')'");
                cleanup(new_node);
                return NULL;
            }
            return new_node;
        }
        default:
            break;
    }

    return NULL;
}

static struct node *parse_multiply(const struct list *tokens, size_t *index) {
    struct node *left = parse_primary(tokens, index);

    struct token *current = list_get(tokens, *index);
    while (current->type == MULTIPLY || current->type == DIVIDE) {
        (*index)++;

        struct node *right = parse_primary(tokens, index);
        if (right == NULL) {
            puts("Error: * and / takes 2 operands, only 1 found.");
            cleanup(left);
            cleanup(right);
            return NULL;
        }

        struct node *new_node = create_node(current);
        new_node->left = left;
        new_node->right = right;
        new_node->node_type = INTERNAL;

        current = list_get(tokens, *index);
        left = new_node;
    }

    return left;
}

static struct node *parse_add(const struct list *tokens, size_t *index) {
    struct node *left = parse_multiply(tokens, index);

    struct token *current = list_get(tokens, *index);
    while (current->type == PLUS || current->type == MINUS) {
        (*index)++;

        struct node *right = parse_multiply(tokens, index);
        if (right == NULL) {
            puts("Error: + and - takes 2 operands, only 1 found.");
            cleanup(left);
            cleanup(right);
            return NULL;
        }

        struct node *new_node = create_node(current);
        new_node->left = left;
        new_node->right = right;
        new_node->node_type = INTERNAL;

        current = list_get(tokens, *index);
        left = new_node;
    }

    return left;
}

static struct node *build_ast(const struct list *tokens, size_t *index) {
    return parse_add(tokens, index);
}

struct node *parse_root(const struct list *tokens) {
    /**
     * 1 + 2 * 3
     * =>
     *     +
     *    / \
     *   1   *
     *      / \
     *     2   3
     */
    size_t index = 0;
    struct node *root = build_ast(tokens, &index);
    return root;
}

void cleanup(struct node *node) {
    if (node == NULL) {
        return;
    }

    cleanup(node->left);
    cleanup(node->right);
    free(node);
}
