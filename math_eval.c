#include "eval.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "linenoise.h"

int main() {
    linenoiseHistorySetMaxLen(10);

    puts("Math Evaluator");
    puts("q to exit");
    char *input = NULL;
    while ((input = linenoise("Input an expression to evaluate: ")) != NULL) {
        if (strcmp("q", input) == 0) {
            free(input);
            break;
        }
        linenoiseHistoryAdd(input);
        eval(input);
        free(input);
    }
}
