#include <getopt.h>
#include <stdio.h>
#include "server.h"


// Global parameters
static const char board_type[] = {'c', 't', 'h', 's'};
static const int number_of_board_type = 4;
static int (*board_size_constraint[])(int) = {
  c_constraint,
  t_constraint,
  h_constraint,
  s_constraint
};

static char t = 'c';
static int m = 2;
static int p = 0;
static int d = 0;


////////////////////////////////////////////////////////////////
// Function for checking the options of the program
// exit if an error occur

int check_type_board()
{
  for (int i = 0; i < number_of_board_type; i++)
    if (board_type[i] == t)
      return i;
  return -1;
}

int check_size_board()
{
  int i = check_type_board();
  return i >= 0 && i < number_of_board_type && board_size_constraint[i](m);
}

void check_opts()
{
  if (!check_size_board()) {
    fprintf(stderr, "Usage: wrong size or/and board type\n");
    exit(1);
  }
}

////////////////////////////////////////////////////////////////
// Function for parsing the options of the program
// Currently available options are :
// -t : type of board
// -m : size of board
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

int main(int argc , char * argv[]){

  parse_opts(argc, argv);
  check_opts();

  struct server *server;
  server = initialize_server(argv[optind], argv[optind + 1], m, t);
  
  run_server(server, p, d);
  free_server(server);
  return 0;
}