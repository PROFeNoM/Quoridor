#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>

typedef void* (*Copy)(const void*);
typedef void (*Release)(void*);

struct list;

// Return a struct list representing the empty list
struct list* list__empty(Copy copy, Release free_element);

// Returns non-zero if list is empty, 0 otherwise
int list__is_empty(const struct list* lst);

// Returns the number of elements in lst
long unsigned int list__size(const struct list* lst);

// Adds c to lst
void list__add(struct list* lst, void* c);

// Returns the ith element of lst
void* list__get(const struct list* lst, size_t i);

// Changes the ith element of lst
void list__change(const struct list* lst, size_t i, void* c);

// Frees the memory allocated for struct list lst
void list__free(struct list* lst);

#endif //_LIST_H
