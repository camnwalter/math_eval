CC = gcc
CFLAGS = -Wall -Wextra
CFLAGS_DEB = -O0 -ggdb
EXTRAS = -lm

SHELL_NAME = math_eval

SRCS =  math_eval.c
SRCS += ast.c ast.h
SRCS += eval.c eval.h
SRCS += lexer.c lexer.h
SRCS += list.c list.h
SRCS += token.c token.h


eval:
	$(CC) $(CFLAGS) $(SRCS) -o $(SHELL_NAME) $(EXTRAS)

debug:
	$(CC) $(CFLAGS) $(CFLAGS_DEB) $(SRCS) -o $(SHELL_NAME) $(EXTRAS)