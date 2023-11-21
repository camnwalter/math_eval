#pragma once

#include "list.h"

enum token_type {
    // literals
    NUMBER,
    // add variable support later?

    // delimiters
    L_PAREN,
    R_PAREN,

    // binary operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,

    // unary operators
    U_MINUS,

    EOL,
    INVALID = -1,
};

struct token {
    enum token_type type;
    double data;
};

enum token_type char_to_type(char);
char *type_to_string(enum token_type);

