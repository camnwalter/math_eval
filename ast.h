#pragma once

#include "token.h"
#include <stdbool.h>

enum node_type {
    LEAF,
    INTERNAL,
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
struct node *parse_root(const struct list *);
void cleanup(struct node *);

