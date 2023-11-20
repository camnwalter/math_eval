#include "eval.h"
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

#define MAX_INPUT_LENGTH 256

static volatile bool running = true;

void handle_signals() {
    running = false;
}

int main() {
    struct sigaction sig;
    sig.sa_handler = handle_signals;
    sigaction(SIGINT, &sig, NULL);

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