#ifndef MATH_EVAL_AST_H
#define MATH_EVAL_AST_H

#include "token.h"
#include <stdbool.h>

enum node_type {
    LEAF,
    INTERNAL,
    ROOT
};

struct node {
    enum token_type token;
    enum node_type node_type;
    double val; // value of node
    struct node *children[2]; // array of children, 2 is max for binary ops
};

bool is_unary_op(enum token_type);
bool is_binary_op(enum token_type);
struct node *parse_root(const struct list *);
void cleanup(struct node *);

#endif //MATH_EVAL_AST_H
