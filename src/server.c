#include <dlfcn.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph_pattern.h"

// Global parameters
static char t = 'c'; // Global seed for the random number generator
static int m = 2;

////////////////////////////////////////////////////////////////
// Function for parsing the options of the program
// Currently available options are :
// -t : type of board
// -m : size of board

void parse_opts(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "t:m")) != -1) {
    switch (opt) {
    case 't':
      t = optarg;
      break;
    case 'm':
      m = atoi(opt);
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
  
  parse_opts(argc, argv);

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
  int (*f_x)(int) = dlsym(joueur_1,"f");
  printf("%d\n",(*f_x)(5));
  gsl_spmatrix * matrix = square_graph(m);
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      printf(" %f ",gsl_spmatrix_get(matrix,i,j));
    }
    printf("\n");
  }
  return 0;
}
  
