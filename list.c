#include "list.h"
#include <string.h>

struct list {
    void **data;
    size_t size;
    size_t remaining;
};

static bool list_resize(struct list *);
static bool ensure_capacity(struct list *);

/**
 * creates a new list with an initial capacity
 * @return the list
 */
struct list *list_init(size_t initial_capacity) {
    struct list *list = malloc(sizeof *list);
    if (list == NULL) {
        return NULL;
    }

    list->data = calloc(initial_capacity, sizeof *list->data);
    if (list->data == NULL) {
        free(list);
        return NULL;
    }

    list->size = 0;
    list->remaining = initial_capacity;

    return list;
}

/**
 * adds data to end of list
 * @return whether the data was added
 */
bool list_add(struct list *list, void *data) {
    if (!ensure_capacity(list)) {
        return false;
    }

    list->data[list->size] = data;
    list->size++;
    list->remaining--;

    return true;
}

void *list_get(const struct list *list, size_t index) {
    if (index >= list_size(list)) {
        return NULL;
    }

    return list->data[index];
}

/**
 * removes list data at index [index]
 * @return the deleted data
 */
void *list_remove(struct list *list, size_t index) {
    if (index >= list_size(list)) {
        return NULL;
    }

    void *data = list_get(list, index);
    size_t elems_to_shift = list_size(list) - index - 1;
    memmove(&data, &list->data[index + 1], elems_to_shift * sizeof *list->data);
    list->data[list->size - 1] = NULL;
    list->size--;
    list->remaining++;

    return data;
}

/**
 * clears list, zeroing out the bytes
 */
void list_clear(struct list *list) {
    for (int i = list->size - 1; i >= 0; i--) {
        free(list_remove(list, i));
    }
}

/**
 * frees list and all of its data members
 */
void list_free(struct list *list) {
    list_clear(list);

    free(list->data);
    free(list);
}

size_t list_size(const struct list *list) {
    return list->size;
}

/**
 * checks whether the list needs to be resized or not
 * @return whether the list was able to be resized or no new capacity needed
 */
static bool ensure_capacity(struct list *list) {
    if (list->remaining == 0) {
        return list_resize(list);
    }
    return true;
}

/**
 * resizes the list to allow for more capacity
 * @return whether the list was able to be resized
 */
static bool list_resize(struct list *list) {
    size_t new_size = list->size * 2;
    if (new_size == 0) {
        new_size = 1;
    }

    void **new_data = realloc(list->data, new_size * sizeof *list->data);
    if (new_data == NULL) {
        return false;
    }

    list->remaining = new_size - list->size;
    list->data = new_data;
    memset(&list->data[list->size], 0, list->remaining * sizeof *list->data);

    return true;
}