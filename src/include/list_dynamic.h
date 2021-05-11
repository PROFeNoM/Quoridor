#ifndef _LIST_DYNAMIC_H
#define _LIST_DYNAMIC_H

#include "list.h"

/**
 * Dynamic List Structure
 * @param l Array of void*
 * @param capacity Total capacity of the list
 * @param size Current size of the list
 * @param cp Copy function
 * @param del Release function
 */
struct list {
	void** l;
	size_t capacity;
	size_t size;
	Copy cp;
	Release del;
};

#endif //_LIST_DYNAMIC_H
