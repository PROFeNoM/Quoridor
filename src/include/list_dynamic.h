#ifndef _LIST_DYNAMIC_H
#define _LIST_DYNAMIC_H

#include "list.h"

struct list {
	void** l;
	size_t capacity;
	size_t size;
	Copy cp;
	Release del;
};

#endif //_LIST_DYNAMIC_H
