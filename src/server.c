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

  parse_opts(argc-2, argv);

  struct graph_t * graph = initialize_graph(int m);
  struct player_server players[2];

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
  
  players[BLACK].get_player_name = dlsym(joueur_1,"get_player_name");
  players[BLACK].initialize  = dlsym(joueur_1,"initialize");
  players[BLACK].play = dlsym(joueur_1,"play");
  players[BLACK].finalize = dlsym(joueur_1,"finalize");
  players[WHITE].get_player_name = dlsym(joueur_2,"get_player_name");
  players[WHITE].initialize  = dlsym(joueur_2,"initialize");
  players[WHITE].play = dlsym(joueur_2,"play");
  players[WHITE].finalize = dlsym(joueur_2,"finalize");

 
  
  players[BLACK].initialize(BLACK,graph_copy(graph),22);
  players[WHITE].initialize(WHITE,graph_copy(graph),22);
 

  struct move_t move = {.m = 0 , .e = {-1,-1} ,.t = NO_TYPE , .c=WHITE};
  int tour = 0 ;
  
  while (true){
    move = players[get_next_player(move.c)].play(move);
    graph_update(graph,move);
    if (is_winning(players[move.c], graph, move.c)){
      break ;
    }
  }
  players[BLACK].finalize();
  players[WHITE].finalize();
   

  dlclose(joueur_1);
  dlclose(joueur_2);
  return 0;
}





  
/*
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      printf(" %f ",gsl_spmatrix_get(matrix,i,j));
    }
    printf("\n");
  }
  gsl_spmatrix * matrix_pos = matrix_position(m);

  struct graph_t graph ={.num_vertices = m*m , .t = matrix , .o = matrix_pos, .p={SIZE_MAX,SIZE_MAX}};
  */
