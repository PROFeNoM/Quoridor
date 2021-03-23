#include <getopt.h>
#include <stdio.h>
#include "server.h"


// Global parameters
static char t = 'c';
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
      t = *optarg;
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

  struct server *server;
  server = initialize_server(argv[argc - 2], argv[argc - 1], m, t);
  
  run_server(server);
  
  return 0;
}