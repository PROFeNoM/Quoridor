#ifndef _UTILS_H_
#define _UTILS_H_

#include <dlfcn.h>
#include "graph.h"

struct graph_t * graph_copy(struct graph_t *);

void graph_free(struct graph_t *);

void *load_library(char *);

void *load_function(void *, const char *);

enum color_t get_next_player(enum color_t);

int is_winning(struct graph_t *, enum color_t, size_t);

#endif
