#ifndef MATH_EVAL_TOKEN_H
#define MATH_EVAL_TOKEN_H

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

    IDENTITY,
    EOL,
    INVALID = -1,
};

struct token {
    enum token_type type;
    double data;
};

enum token_type char_to_type(char);
char type_to_char(enum token_type);

#endif //MATH_EVAL_TOKEN_H
