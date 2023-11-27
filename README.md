# math_eval
Simple C program that evaluates math expressions.

This was written over Thanksgiving Break 2023, so it's not the best :)

## Operations supported:
- Addition
- Subtraction
- Multiplication
  - Implicit multiplication is also supported, but doesn't have higher precedence.
- Division
- Exponentiation
- Parentheses

## Potential additional features, in order of how likely I am to do them...
- Variables
- Trig functions

## How to build:
- run `make`, or `make debug` for debugging purposes.
- run the executable via `./math_eval`.

## Credits
- [CS429 CI Lab](https://github.com/CS429-S2023/CI-Lab) for some ideas on how to
set up the AST
- https://github.com/tlaceby/guide-to-interpreters-series
