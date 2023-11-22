#include "eval.h"
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

#define MAX_INPUT_LENGTH 256

int main() {
    puts("Math Evaluator");
    char input[MAX_INPUT_LENGTH];
    char *output = NULL;
    while ((output = fgets(input, MAX_INPUT_LENGTH, stdin)) != NULL) {
        puts("Input an expression to be evaluated...");
        eval(output);
    }
    return 0;
}
