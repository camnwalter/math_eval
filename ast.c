#include <stdio.h>
#include "ast.h"
#include "token.h"

// ORDER OF OPERATIONS:
// 1. Parentheses
// 2. Exponents
// 3. Multiplication/Division
// 4. Addition/Subtraction
static struct node *parse_primary(const struct list *tokens, size_t *index);

static struct node *parse_exponent(const struct list *tokens, size_t *index);

static struct node *parse_multiply(const struct list *tokens, size_t *index);

static struct node *parse_add(const struct list *tokens, size_t *index);

bool is_unary_op(enum token_type type) {
    return type == U_MINUS;
}

bool is_binary_op(enum token_type type) {
    return type >= PLUS && type <= POWER;
}

static struct node *create_node(struct token *token) {
    struct node *node = malloc(sizeof *node);
    if (node == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    node->token = token->type;
    node->val = token->data;
    node->left = NULL;
    node->right = NULL;
    node->node_type = DEFAULT;
    return node;
}

static struct node *parse_primary(const struct list *tokens, size_t *index) {
    struct token *current = list_get(tokens, *index);
    if (current == NULL) {
        return NULL;
    }

    switch (current->type) {
        case U_MINUS: {
            (*index)++;
            struct node *new_node = create_node(current);
            new_node->left = parse_add(tokens, index);
            if (new_node->left == NULL) {
                puts("Error: - takes 1 or 2 operands, none found.");
                cleanup(new_node);
                return NULL;
            }
            new_node->node_type = INTERNAL;
            return new_node;
        }
        case NUMBER: {
            (*index)++;
            struct node *new_node = create_node(current);
            new_node->node_type = LEAF;
            return new_node;
        }
        case L_PAREN: {
            (*index)++;
            struct node *new_node = parse_add(tokens, index);
            if (((struct token *) list_get(tokens, *index))->type != R_PAREN) {
                puts("Expected ')'");
                cleanup(new_node);
                return NULL;
            }
            (*index)++;
            return new_node;
        }
        case R_PAREN: {
            puts("Expected token: ')'");
            return NULL;
        }
        default:
            break;
    }

    return NULL;
}

static struct node *parse_exponent(const struct list *tokens, size_t *index) {
    struct node *left = parse_primary(tokens, index);

    struct token *current = list_get(tokens, *index);
    if (current == NULL) {
        return NULL;
    }
    while (current->type == POWER) {
        (*index)++;
        if (left == NULL) {
            puts("Error: ^ takes 2 operands, none found.");
            return NULL;
        }

        struct node *right = parse_primary(tokens, index);
        if (right == NULL) {
            puts("Error: ^ takes 2 operands, only 1 found.");
            cleanup(left);
            return NULL;
        }

        struct node *new_node = create_node(current);
        new_node->left = left;
        new_node->right = right;
        new_node->node_type = INTERNAL;

        current = list_get(tokens, *index);
        if (current == NULL) {
            return NULL;
        }
        left = new_node;
    }

    return left;
}

static struct node *parse_multiply(const struct list *tokens, size_t *index) {
    struct node *left = parse_exponent(tokens, index);

    struct token *current = list_get(tokens, *index);
    if (current == NULL) {
        return NULL;
    }
    while (current->type == MULTIPLY || current->type == DIVIDE) {
        (*index)++;
        if (left == NULL) {
            puts("Error: * and / take 2 operands, none found.");
            return NULL;
        }

        struct node *right = parse_exponent(tokens, index);
        if (right == NULL) {
            puts("Error: * and / take 2 operands, only 1 found.");
            cleanup(left);
            return NULL;
        }

        struct node *new_node = create_node(current);
        new_node->left = left;
        new_node->right = right;
        new_node->node_type = INTERNAL;

        current = list_get(tokens, *index);
        if (current == NULL) {
            return NULL;
        }
        left = new_node;
    }

    return left;
}

static struct node *parse_add(const struct list *tokens, size_t *index) {
    struct node *left = parse_multiply(tokens, index);

    struct token *current = list_get(tokens, *index);
    if (current == NULL) {
        return NULL;
    }
    while (current->type == PLUS || current->type == MINUS) {
        (*index)++;
        if (left == NULL) {
            puts("Error: + and - take 2 operands, none found.");
            return NULL;
        }

        struct node *right = parse_multiply(tokens, index);
        if (right == NULL) {
            puts("Error: + and - take 2 operands, only 1 found.");
            cleanup(left);
            return NULL;
        }

        struct node *new_node = create_node(current);
        new_node->left = left;
        new_node->right = right;
        new_node->node_type = INTERNAL;

        current = list_get(tokens, *index);
        if (current == NULL) {
            return NULL;
        }
        left = new_node;
    }

    return left;
}

struct node *parse_root(const struct list *tokens) {
    size_t index = 0;
    struct node *root = parse_add(tokens, &index);
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
