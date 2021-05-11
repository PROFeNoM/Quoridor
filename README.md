# Projet Quoridor

- [Project Subject Page](https://www.labri.fr/perso/renault/working/teaching/projets/2020-21-S6-C-Quoridor.php)

- [Thor Page](https://thor.enseirb-matmeca.fr/ruby/projects/projetss6-quor)

# Description

The objective of the project is to implement a set of functions allowing two players to play a game of Quoridor. The code is broken down into two distinct parts:
- A set of clients all implementing a common interface, managing their own game board and playing each according to their own objective. These clients are automatic: they have to decide which move to play using an algorithm, without any external intervention.
- A game server organizing a game, making each client play in turn by sending it the moves played by its opponent, recording the client's move in turn and notifying the clients of the end of the game.


# Requirements

- The ```make``` command 
- GCC compiler 
- Standard C libraries and headers
- GSL - GNU Scientific Library

In case you want to have some documentation, these libraries are needed:
- Doxygen 
- Graphiz

# How to compile

Go to the root of the repository and run

```shell
$ make install
```

# How to run a game

After compilation, go to the root of the repository. Running a game uses the following syntax

```shell
$ ./install/server -m [M] -t [T] -d [D] -p PATH_CLIENT_1 PATH_CLIENT_2
```

- The ```-m``` option allows to specify the width of the game board (e.g. ```-m 10```).
- The ```-t``` option allows to specify the shape of the game board (square grid ```c```, toric grid ```t```, chopped grid ```h``` or snake grid ```s```) (e.g. ```-t h```).
- The ```-d``` (or ```--delay```) option allows to add a delay (in seconds) between the players move (e.g. ```-d 1```).
- The ```-p``` (or ```--print```) option allows to have a visual representation of the ongoing game. When using this option, please make sure to have a terminal window size large enough.
- The clients' path are passed as parameters in the order 1st player / 2nd player (e.g. ```./install/ForrestGump_1.so```). Please make sure not to use twice the same client.

The optional parameters can be placed in any order on the command line.

# Players implemented

## Alpha Beta Player

- Explication: This player is based on the Minimax algorithm with Alpha-Beta Pruning. 
- Path from root: ```./install/1alphaBetaPlayer_1.so``` or ```./install/1alphaBetaPlayer_2.so```

## Intelligent Anti Bolt Player

- Explication: Prevent the opponent to go in a straight line by placing walls, and move based on a Dijkstra Algorithm
- Path from root: ```./install/1IntelligentAntiBolt_1.so``` or ```./install/1IntelligentAntiBolt_2.so```

## Anti Bolt Player

- Explication: Prevent the opponent to go in a straight-line by placing walls and move in a straight line.
- Path from root: ```./install/AntiBolt_1.so``` or ```./install/AntiBolt_2.so```

## Forrest Gump Player

- Explication: This player goes in a straight-line to the opponent area.
- Path from root: ```./install/ForrestGump_1.so``` or ```./install/ForrestGump_2.so```

## Random Player

- Explication: This player moves randomly.
- Path from root: ```./install/random_1.so``` or ```./install/random_2.so```

# Usage example

- Using the previously defined syntax, a game could be run as follows:

```shell
$ ./install/server ./install/1alphaBetaPlayer_1.so ./install/1IntelligentAntiBolt_1.so -p -t c -m 10 
```

- When using twice the same player implementation, a game could be run as follows: 

```shell
$ ./install/server install/1IntelligentAntiBolt_1.so install/1IntelligentAntiBolt_2.so -m 4
```

# How to run tests

0. Compile the project as explained previously

1. At the root of the repository, run

```shell
$ ./install/alltests
```

# How to generate the project documentation

1. Go to the root of the repository and run

```shell
$ make docs
```

2. Open ```./docs/html/index.html``` with your most beloved browser

# How to clean the repository

- Go to the root of the repository

- To clean the repository to its initial state, run

```shell
$ make mrproper
```

- To keep the project documentation and remove everything which was compiled, run

```shell
$ make clean
```

# Authors

- Alexandre Choura
- Léo Guerin
- Lucas Marais
- Jean-François Sornay
