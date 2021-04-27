#include "utils.h"
#include "dijsktra.h"

struct dijsktra_t {

  size_t dist;
  size_t prec;
};


struct dijsktra_t * init_dijsktra(size_t dist, size_t prec){

  struct dijsktra_t *  d = malloc(sizeof(struct dijsktra_t));
  d->dist = dist;
  d->prec = prec;
  return d;
}

void free_dijsktra(struct dijsktra_t * d){

  free(d);
}

  

int all_visited(gsl_spmatrix_uint * visited){

  for (size_t i = 0 ; i < visited->size2 ; i++){
    if (gsl_spmatrix_uint_get(visited,0,i) == 0){
      return 0;
    }
  }
  return 1;
}

gsl_spmatrix_uint *  init_visited(size_t num){

  gsl_spmatrix_uint * visited = gsl_spmatrix_uint_alloc(1,num);
  for (size_t i = 0 ; i<num ; i++){
    gsl_spmatrix_uint_set(visited,0,i,0);
  }
  return visited;
}

gsl_spmatrix_uint * init_dist(size_t num){

  gsl_spmatrix_uint * dist = gsl_spmatrix_uint_alloc(1,num);
  for (size_t i = 0 ; i<num ; i++){
    gsl_spmatrix_uint_set(dist,0,i,num+1);
  }
  return dist;
}

gsl_spmatrix_uint * init_prec(size_t num){

  gsl_spmatrix_uint * prec = gsl_spmatrix_uint_alloc(1,num);
  for (size_t i = 0 ; i<num ; i++){
    gsl_spmatrix_uint_set(prec,0,i,i);
  }
  return prec;
}

int is_visited(gsl_spmatrix_uint * visited,size_t index){

  return gsl_spmatrix_uint_get(visited,0,index) != 0;
}

size_t get_dist_min_not_visited(gsl_spmatrix_uint * dist,gsl_spmatrix_uint * visited){

  size_t dist_min = dist->size2 * dist->size2;
  size_t index_min = 0;
  for (size_t i = 0 ; i <dist->size2 ; i++){
    if (gsl_spmatrix_uint_get(dist,0,i) <= dist_min && (! is_visited(visited,i))){
      dist_min = gsl_spmatrix_uint_get(dist,0,i);
      index_min = i;
    }
  }
  return index_min;
}
  


size_t dijsktra(struct graph_t * graph,size_t src,size_t dst){

  size_t num = graph->num_vertices;
  gsl_spmatrix_uint * visited = init_visited(num);
  gsl_spmatrix_uint * dist = init_dist(num);
  gsl_spmatrix_uint * prec = init_prec(num);
  gsl_spmatrix_uint_set(dist,0,src,0);
  size_t index = 0;
  size_t length;
  while (! all_visited(visited)){
    index = get_dist_min_not_visited(dist,visited);
    gsl_spmatrix_uint_set(visited,0,index,1);
    /*
    if (index == dst){
      break;
    }
    */
    for (size_t i = 0 ; i < num ; i++){
      if (is_connected(graph,index,i)){
	  length = gsl_spmatrix_uint_get(dist,0,index)+1;
	  if (length < gsl_spmatrix_uint_get(dist,0,i)){
	    gsl_spmatrix_uint_set(dist,0,i,length);
	    gsl_spmatrix_uint_set(prec,0,i,index);
	    
	  }
      }
    }
  }
  gsl_spmatrix_uint_free(visited);
  /*
  size_t l = num + 1;
  size_t vertice = dst;
  size_t origin = dst;
  //printf("\n dst = %ld \n",origin);
  size_t compt = 0;
  while ( origin != src && compt < num){

    vertice = origin;
    origin = gsl_spmatrix_uint_get(prec,0,origin);
  }
 
  l = gsl_spmatrix_uint_get(dist,0,dst);
  
  
  struct dijsktra_t * d = init_dijsktra(l,vertice);
  return d;
  */
  length = gsl_spmatrix_uint_get(dist,0,dst);
  gsl_spmatrix_uint_free(dist);
  gsl_spmatrix_uint_free(prec);
  return length;
}

/*
size_t mini_dijsktra(struct graph_t * graph, size_t position, enum color_t id){

  
  size_t vertice;
  size_t num =graph->num_vertices; 
  size_t length = num+1;
  //size_t choose;
  for (size_t i = 0 ; i < num ; i++){
    
    if (is_owned(graph,get_next_player(id),i)){
      //printf(" %ld ",i);
      struct dijsktra_t * d = dijsktra(graph,position,i);

      if (d->dist <= length){
	length = d->dist;
	vertice = d->prec;
	//choose = i;
      }
      free_dijsktra(d);
    }
  }
  return vertice;
}
*/



 
	
	
	    
    
