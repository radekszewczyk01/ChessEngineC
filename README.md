# Chess Engine in C

This repository contains a simple yet effective chess engine written in C. The engine utilizes the alpha-beta pruning algorithm for optimizing the search process. It is capable of generating legal moves, evaluating board positions, and determining game states such as checkmate and stalemate.

## Features

- **Alpha-Beta Pruning**: Efficient search algorithm to minimize the number of nodes evaluated.
- **Move Generation**: Generates all legal moves for a given board position.
- **Position Evaluation**: Evaluates board positions to determine the best moves.
- **Checkmate and Stalemate Detection**: Identifies checkmate and stalemate conditions.

## Getting Started

To get started with the chess engine, you can clone the repository and compile the code using a C compiler.

### Prerequisites

- C Compiler (e.g., gcc)
- Make (optional, for building with a Makefile)

### Installation


   ```bash
   git clone https://github.com/yourusername/ChessEngineC.git
   cd ChessEngineC
   cd Szachy
   gcc -o chess_engine main.c
   .\chess_2_0
   bash
   ```

### Usage
The main functions and their purposes are as follows:

`generator_legalnych_ruchow`
Generates a list of all legal moves for a given board position and player color.

`pat_mat`
Determines the game state (ongoing, stalemate, or checkmate).

`ocen_pozycje`
Evaluates the board position to assign a numerical score.



### Acknowledgements
Special thanks to the author of this video, which helped me in understanding the construction of a simple chess engine.
