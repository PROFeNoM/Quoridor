#include "utils.h"

struct graph_t* graph_copy(struct graph_t* graph)
{
    struct graph_t* cp = malloc(sizeof(struct graph_t));

    cp->num_vertices = graph->num_vertices;

    cp->t = gsl_spmatrix_alloc(graph->num_vertices, graph->num_vertices);
    gsl_spmatrix_memcpy(cp->t, graph->t);

    cp->o = gsl_spmatrix_alloc(2, graph->num_vertices);
    gsl_spmatrix_memcpy(cp->o, graph->o);

    return cp;
}


void graph_free(struct graph_t *graph)
{
  gsl_spmatrix_free(graph->t);
  gsl_spmatrix_free(graph->o);
  free(graph);
}

void *load_library(char *path)
{
  void *lib;
  lib = dlopen(path, RTLD_LAZY);
  if (!lib)
    return NULL;
  return lib;
}

void *load_function(void *lib, const char *name)
{
  void *function = dlsym(lib, name);
  if (!function)
    return NULL;
  return function;
}

enum color_t get_next_player(enum color_t id)
{
  return (id == BLACK) ? WHITE : BLACK;
}

int is_winning(struct graph_t *graph, enum color_t player_id, size_t position)
{
  return gsl_spmatrix_get(graph->o, get_next_player(player_id), position);
}