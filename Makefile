CC = gcc
CFLAGS = -Wall -Wextra
CFLAGS_DEB = -O0 -ggdb
EXTRAS = -lm

SHELL_NAME = math_eval

SRCS =  math_eval.c
SRCS += ast.c
SRCS += eval.c
SRCS += lexer.c
SRCS += list.c
SRCS += token.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(SHELL_NAME) $(EXTRAS)

debug:
	$(CC) $(CFLAGS) $(CFLAGS_DEB) $(SRCS) -o $(SHELL_NAME) $(EXTRAS)