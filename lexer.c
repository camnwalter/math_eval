#include "lexer.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void get_number(struct list *tokens, char *input, size_t *pos) {
    char *output;
    char *input_offset = input + *pos;
    double val = strtod(input_offset, &output);

    if (val == 0 && output == input_offset) {
        // sanity check
        abort();
    }

    *pos += output - input_offset;

    struct token *representation = malloc(sizeof *representation);
    if (representation == NULL) {
        return;
    }

    representation->data = val;
    representation->type = NUMBER;
    list_add(tokens, representation);
}

struct list *convert_to_tokens(char *input) {
    size_t pos = 0;
    struct list *tokens = list_init(0);

    while (pos < strlen(input)) {
        char current = input[pos];
        if (current == '\n') {
            struct token *representation = malloc(sizeof *representation);
            if (representation == NULL) {
                break;
            }
            representation->data = 0;
            representation->type = EOL;

            list_add(tokens, representation);
            break;
        }

        if (current == ' ' || current == '\t') {
            pos++;
        } else if (strchr(".0123456789", current) != NULL) {
            get_number(tokens, input, &pos);
        } else {
            enum token_type type = char_to_type(current);
            if (type == INVALID) {
                printf("Unknown token: %c\n", current);
                break;
            }

            struct token *representation = malloc(sizeof *representation);
            if (representation == NULL) {
                break;
            }
            representation->data = 0;
            representation->type = type;

            list_add(tokens, representation);
            pos++;
        }
    }

    return tokens;
}