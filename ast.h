#pragma once

#include "token.h"
#include <stdbool.h>

enum node_type {
    LEAF,
    INTERNAL,
    DEFAULT = -1, // for debugging purposes
};

struct node {
    enum token_type token;
    enum node_type node_type;
    double val; // value of node
    struct node *left;
    struct node *right;
};

bool is_unary_op(enum token_type);
bool is_binary_op(enum token_type);
bool parse_root(const struct list *, struct node **);
void cleanup(struct node *);

