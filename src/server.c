#include <dlfcn.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph_pattern.h"

// Global parameters
static char * t = "c"; // Global seed for the random number generator
static int m = 2;

////////////////////////////////////////////////////////////////
// Function for parsing the options of the program
// Currently available options are :
// -t : type of board
// -m : size of board

void parse_opts(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "m:t")) != -1) {
    switch (opt) {
    case 't':
      t = optarg;
      break;
    case 'm':
      m = atoi(optarg);
      break;
    default: /* '?' */
      fprintf(stderr, "Usage: %s [-t type of board] [-m size of board]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


int main(int argc , char * argv[]){

  void * joueur_1;
  void * joueur_2;
  
  joueur_1 = dlopen(argv[argc-2],RTLD_LAZY);
  if (!joueur_1){
    fputs (dlerror(),stderr);
    exit(1);
  }
  joueur_2 = dlopen(argv[argc-1],RTLD_LAZY);
  if (!joueur_2){
    fputs (dlerror(),stderr);
    exit(1);
  }
  
  int (* get_player_name_j1)(void) = dlsym(joueur_1,"get_player_name");
  void (*initialize_color_j1)(enum color_id) = dlsym(joueur_1,"initialize_color");
  struct move_t (*play_j1)(struct move_t) = dlsym(joueur_1,"play");
  //void (*finalize_j1)(void) = dlsym(joueur_1,"finalize");
  

  parse_opts(argc-2, argv);
  
  gsl_spmatrix * matrix = square_graph(m);
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      printf(" %f ",gsl_spmatrix_get(matrix,i,j));
    }
    printf("\n");
  }
  gsl_spmatrix * matrix_pos = matrix_position(m);

  struct graph_t graph ={.num_vertices = m*m , .t = matrix , .o = matrix_pos, .p={SIZE_MAX,SIZE_MAX}};
  

  dlclose(joueur_1);
  dlclose(joueur_2);
  return 0;
}
  
