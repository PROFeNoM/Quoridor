#ifndef _LIST_DYNAMIC_H
#define _LIST_DYNAMIC_H

#include "list.h"

// Dynamic list structure
struct list {
	void** l;  // Array of void*
	size_t capacity;  // Total capacity of the list
	size_t size;  // Current size of the list
	Copy cp;  // Copy function
	Release del;  // Release function
};

#endif //_LIST_DYNAMIC_H
