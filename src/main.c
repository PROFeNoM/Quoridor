#include <getopt.h>
#include <stdio.h>
#include "server.h"


/**
 * Global parameters
 */
static char t = 'c';    // Type of board
static int m = 2;       // Size of board
static int p = 0;       // Boolean print
static int d = 0;       // Delay between turns



/**
 * Function for checking the options of the program
 * exit if an error occurs
 */
void check_opts()
{
  if (!abide_graph_constraint(t, m)) {
    fprintf(stderr, "Usage: wrong size or/and board type\n");
    exit(1);
  }
}

/**
 * Function for parsing the options of the program
 * Currently available options are :
 * -t : type of board
 * -m : size of board
 * -p : if server can display
 * -d : delay between turns
 */
void parse_opts(int argc, char* argv[]) {
  int opt;
  int option_index = 0;
  static struct option long_options[] = {
      {"print", no_argument, NULL, 'p'},
      {"delay", required_argument, NULL, 'd'},
  };

  while ((opt = getopt_long(argc, argv, "m:t:pd:", long_options, &option_index)) != -1)
  {
    switch (opt) {
    case 'p':
      p = 1;
      break;
    case 'd':
      d = abs(atoi(optarg));
      break;
    case 't':
      t = *optarg;
      break;
    case 'm':
      m = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-t type of board] [-m size of board]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc , char * argv[])
{
  parse_opts(argc, argv);
  check_opts();

  struct server *server;
  server = initialize_server(argv[optind], argv[optind + 1], m, t);
  
  run_server(server, p, d);
  free_server(server);
  return 0;
}