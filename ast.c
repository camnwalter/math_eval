#include "ast.h"
#include "token.h"

static struct token *current_token;

// Hierarchy:
// 1. Expression - the whole thing

static struct node *parse_expression(const struct list *, size_t *);

static struct node *parse_term(const struct list *, size_t *);

static struct node *parse_exponent(const struct list *, size_t *);

static struct node *parse_factor(const struct list *, size_t *);

bool is_unary_op(enum token_type type) {
    return type == U_MINUS;
}

bool is_binary_op(enum token_type type) {
    return type >= PLUS && type <= POWER;
}

static void get_next_token(const struct list *tokens, size_t *index) {
    (*index)++;
    current_token = list_get(tokens, *index);
}

struct node *parse_root(const struct list *tokens) {
    size_t index = -1;
    get_next_token(tokens, &index);
    struct node *root = parse_expression(tokens, &index);

    return root;
}

static struct node *parse_expression(const struct list *tokens, size_t *index) {
    struct node *left = parse_term(tokens, index);

    while (current_token->type == PLUS || current_token->type == MINUS ||
           is_unary_op(current_token->type)) {
        struct token *token = current_token;
        get_next_token(tokens, index);
        struct node *right = parse_term(tokens, index);

        struct node *new_node = malloc(sizeof *new_node);
        new_node->token = token->type;
        new_node->node_type = INTERNAL;

        new_node->children[0] = left;
        new_node->children[1] = right;

        left = new_node;
    }

    return left;
}

static struct node *parse_term(const struct list *tokens, size_t *index) {
    struct node *left = parse_exponent(tokens, index);

    while (current_token->type == POWER) {
        struct token *token = current_token;
        get_next_token(tokens, index);
        struct node *right = parse_exponent(tokens, index);

        struct node *new_node = malloc(sizeof *new_node);
        new_node->token = token->type;
        new_node->node_type = INTERNAL;

        new_node->children[0] = left;
        new_node->children[1] = right;

        left = new_node;
    }

    return left;
}

static struct node *parse_exponent(const struct list *tokens, size_t *index) {
    struct node *left = parse_factor(tokens, index);

    while (current_token->type == MULTIPLY || current_token->type == DIVIDE) {
        struct token *token = current_token;
        get_next_token(tokens, index);
        struct node *right = parse_factor(tokens, index);

        struct node *new_node = malloc(sizeof *new_node);
        new_node->token = token->type;
        new_node->node_type = INTERNAL;

        new_node->children[0] = left;
        new_node->children[1] = right;

        left = new_node;
    }

    return left;
}

static struct node *parse_factor(const struct list *tokens, size_t *index) {
    if (current_token->type == NUMBER) {
        struct node *new_node = malloc(sizeof *new_node);
        new_node->token = NUMBER;
        new_node->node_type = LEAF;
        new_node->val = current_token->data;

        new_node->children[0] = NULL;
        new_node->children[1] = NULL;

        get_next_token(tokens, index);
        return new_node;
    } else if (current_token->type == L_PAREN) {
        get_next_token(tokens, index); // Move past the '('
        struct node *new_node = parse_expression(tokens, index);

        if (current_token->type != R_PAREN) {
            // TODO: handle error
        }

        get_next_token(tokens, index); // Move past the ')'
        return new_node;
    } else {
        // TODO: handle error
//        error("Unexpected token");
        return NULL;
    }
}

void cleanup(struct node *node) {
    if (node == NULL) {
        return;
    }

    cleanup(node->children[0]);
    cleanup(node->children[1]);
    free(node);
}