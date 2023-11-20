#ifndef MATH_EVAL_LIST_H
#define MATH_EVAL_LIST_H

#include <stdlib.h>
#include <stdbool.h>

struct list;

struct list *list_init(size_t);
bool list_add(struct list *, void *);
void *list_remove(struct list *, size_t);
void *list_get(const struct list *, size_t);
void list_clear(struct list *);
void list_free(struct list *);
size_t list_size(const struct list *);

#endif //MATH_EVAL_LIST_H
