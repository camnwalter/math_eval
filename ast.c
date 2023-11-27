#include <stdio.h>
#include "ast.h"
#include "token.h"

static bool success = true;

// ORDER OF OPERATIONS:
// 1. Parentheses
// 2. Exponents
// 3. Multiplication/Division. Implicit multiplication is not higher precedence
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

void consume(size_t *index) {
    (*index)++;
}

// NOLINTNEXTLINE(misc-no-recursion)
static struct node *parse_primary(const struct list *tokens, size_t *index) {
    struct token *current = list_get(tokens, *index);
    if (current == NULL) {
        return NULL;
    }

    switch (current->type) {
        case U_MINUS: {
            consume(index);
            struct node *new_node = create_node(current);
            new_node->left = parse_add(tokens, index);
            
            
            if (new_node->left == NULL) {
                if (success) {
                    puts("Error: - takes 1 or 2 operands, none found.");
                    success = false;
                }

                cleanup(new_node);
                return NULL;
            }
            new_node->node_type = INTERNAL;
            return new_node;
        }
        case NUMBER: {
            consume(index);
            struct node *new_node = create_node(current);
            new_node->node_type = LEAF;
            return new_node;
        }
        case L_PAREN: {
            consume(index);
            struct node *new_node = parse_add(tokens, index);
            struct token *token = list_get(tokens, *index);
            if (token->type != R_PAREN) {
                if (success) {
                    puts("Expected ')'");
                    success = false;
                }

                cleanup(new_node);
                return NULL;
            }
            consume(index);
            return new_node;
        }
        case R_PAREN: {
            if (success) {
                puts("Expected token: '('");
                success = false;
            }
            return NULL;
        }
        default:
            break;
    }

    return NULL;
}

// NOLINTNEXTLINE(misc-no-recursion)
static struct node *parse_exponent(const struct list *tokens, size_t *index) {
    struct node *left = parse_primary(tokens, index);

    struct token *current = list_get(tokens, *index);
    while (current != NULL && current->type == POWER) {
        consume(index);
        if (left == NULL) {
            if (success) {
                puts("Error: ^ takes 2 operands, none found.");
                success = false;
            }
            return NULL;
        }

        struct node *right = parse_exponent(tokens, index);
        if (right == NULL) {
            if (success) {
                puts("Error: ^ takes 2 operands, only 1 found.");
                success = false;
            }

            cleanup(left);
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

// NOLINTNEXTLINE(misc-no-recursion)
static struct node *parse_multiply(const struct list *tokens, size_t *index) {
    struct node *left = parse_exponent(tokens, index);

    struct token *current = list_get(tokens, *index);
    while (current != NULL && (current->type == MULTIPLY || current->type == DIVIDE)) {
        consume(index);
        if (left == NULL) {
            if (success) {
                puts("Error: * and / take 2 operands, none found.");
                success = false;
            }
            return NULL;
        }

        struct node *right = parse_exponent(tokens, index);
        if (right == NULL) {
            if (success) {
                puts("Error: * and / take 2 operands, only 1 found.");
                success = false;
            }
            
            cleanup(left);
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

// NOLINTNEXTLINE(misc-no-recursion)
static struct node *parse_add(const struct list *tokens, size_t *index) {
    struct node *left = parse_multiply(tokens, index);

    struct token *current = list_get(tokens, *index);
    while (current != NULL && (current->type == PLUS || current->type == MINUS)) {
        consume(index);
        if (left == NULL) {
            if (success) {
                puts("Error: + and - take 2 operands, none found.");
                success = false;                
            }
            return NULL;
        }

        struct node *right = parse_multiply(tokens, index);
        if (right == NULL) {
            if (success) {
                puts("Error: + and - take 2 operands, only 1 found.");
                success = false;                
            }
            cleanup(left);
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

bool parse_root(const struct list *tokens, struct node **root) {
    size_t index = 0;
    success = true;
    *root = parse_add(tokens, &index);
    return success;
}

// NOLINTNEXTLINE(misc-no-recursion)
void cleanup(struct node *node) {
    if (node == NULL) {
        return;
    }

    cleanup(node->left);
    cleanup(node->right);
    free(node);
}
