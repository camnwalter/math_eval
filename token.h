#pragma once

#include "libs/list.h"

enum token_type {
    NUMBER,
    
    // TODO: variables

    // built-in math functions
    

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

