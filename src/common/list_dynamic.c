#include "../include/list_dynamic.h"


#ifndef INITIAL_CAPACITY
#define INITIAL_CAPACITY 1
#endif // INITIAL_CAPACITY


// Double a dynamic list array size
void double_capacity(struct list* lst)
{
	lst->l = realloc(lst->l, 2 * lst->capacity * sizeof(void*));
	lst->capacity *= 2;
}


// Return a struct list representing the empty list
struct list* list__empty(Copy copy, Release free_element)
{
	struct list* lst = malloc(sizeof(struct list));
	lst->capacity = INITIAL_CAPACITY;
	lst->size = 0;
	lst->l = malloc(INITIAL_CAPACITY * sizeof(void*));
	lst->cp = copy;
	lst->del = free_element;
	return lst;
}


// Returns non-zero if list is empty, 0 otherwise
int list__is_empty(const struct list* lst)
{
	return lst->size == 0;
}


// Returns the number of elements in lst
long unsigned int list__size(const struct list* lst)
{
	return lst->size;
}


// Adds c to lst
void list__add(struct list* lst, void* c)
{
	size_t position = lst->size;

	if (lst->capacity <= lst->size)
		double_capacity(lst);

	*(lst->l + position) = lst->cp(c);

	lst->size++;
}


void* list__get(const struct list* lst, size_t i)
{
	return lst->l[i];
}

// Changes the ith element of lst
void list__change(const struct list* lst, size_t i, void* c)
{
	if (list__size(lst) > i)
	{
		void* previous_data = list__get(lst, i);
		lst->l[i] = lst->cp(c);
		lst->del(previous_data);
	}
}


// Frees the memory allocated for struct list lst
void list__free(struct list* lst)
{
	for (size_t i = 0; i < lst->size; i++)
		lst->del(list__get(lst, i));
	free(lst->l);
	free(lst);
}