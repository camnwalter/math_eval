#include "token.h"

enum token_type char_to_type(char c) {
    switch (c) {
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return MULTIPLY;
        case '/':
            return DIVIDE;
        case '^':
            return POWER;
        case '(':
            return L_PAREN;
        case ')':
            return R_PAREN;
        default:
            return INVALID;
    }
}
char *type_to_string(enum token_type type) {
    switch (type) {
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case MULTIPLY:
            return "*";
        case DIVIDE:
            return "/";
        case POWER:
            return "^";
        case L_PAREN:
            return "(";
        case R_PAREN:
            return ")";
        case NUMBER:
            return "Number";
        default:
            return "Unknown";
    }
}