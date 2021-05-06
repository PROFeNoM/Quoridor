#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>

typedef void* (*Copy)(const void*);
typedef void (*Release)(void*);

struct list;

/**
 * Get a struct list representing the empty list
 * @param copy Copy function
 * @param free_element Release function
 * @return An empty list
 */
struct list* list__empty(Copy copy, Release free_element);

/**
 * Tell if a list is empty
 * @param lst List to check
 * @return Non-zero if list is empty, 0 otherwise
 */
int list__is_empty(const struct list* lst);

/**
 * Get the current size of a list
 * @param lst List to check
 * @return The number of elements in lst
 */
long unsigned int list__size(const struct list* lst);

/**
 * Add c to the list
 * @param lst List in which the element should be added
 * @param c Element to add
 */
void list__add(struct list* lst, void* c);

/**
 * Get the ith element of a list
 * @param lst List
 * @param i Index of the element to get in the list
 * @return The ith element of lst
 */
void* list__get(const struct list* lst, size_t i);

/**
 * Change a list value at a specified index
 * @param lst List to change
 * @param i Index to change
 * @param c New value to exchange with
 */
void list__change(struct list* lst, size_t i, void* c);

/**
 * Frees the memory allocated for struct list lst
 * @param lst List to free
 */
void list__free(struct list* lst);

#endif //_LIST_H
