#include "eval.h"
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

#define MAX_INPUT_LENGTH 256

static volatile bool running = true;

void handle_signals(int signal) {
    if (signal == SIGINT) {
        running = false;
    }
}

int main() {
    signal(SIGINT, handle_signals);

    puts("Math Evaluator");
    char input[MAX_INPUT_LENGTH];
    while (running) {
        puts("Input an expression to be evaluated...");
        fgets(input, MAX_INPUT_LENGTH, stdin);
        if (running) {
            eval(input);
        }
    }
    return 0;
}
