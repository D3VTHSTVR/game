# Cross-Language Tic-Tac-Toe Project

This project is a demonstration of integrating multiple programming languages into one system. It combines **C++**, **Prolog**, and **Racket Scheme** to create a playable Tic-Tac-Toe game where C++ runs the main gameplay loop, Prolog validates moves, and Racket Scheme provides the AI using a minimax algorithm.

The languages communicate through shared text files in the `data/` directory.

## How to Build and Run

### 1. Install dependencies

Make sure you have:

- **g++** with C++17 support
- **SWI-Prolog**

  ```
  brew install swi-prolog
  ```

- **Racket**

  ```
  brew install --cask racket
  ```

### 2. Build the project

Run:

```bash
make
```

This compiles the C++ files into an executable named:

```bash
./game
```

### 3. Run the game

```bash
./game
```

Moves are entered as:

```
row col
```

Player 1 is the human. Player 2 is the Scheme AI.

## How the System Works

### C++ (main engine)

- Runs the UI and game loop
- Writes board state to a shared text file
- Calls Prolog to validate human and AI moves
- Calls Scheme when it’s the AI’s turn
- Reads Scheme output and applies the chosen move

### Prolog (rules checker)

- Reads the board state from `data/current_state.txt`
- Reads the proposed move from `data/move_query.txt`
- Determines whether the move is valid or invalid
- Writes the verdict to `data/move_response.txt`

### Racket Scheme (AI)

- Reads board state from `current_state.txt`
- Uses minimax to pick the strongest move
- Writes the chosen coordinates to `move_response.txt`

## Summary

This project demonstrates a clean cross-language architecture using a simple text-file protocol. Each language handles a different part of the logic: C++ controls the game, Prolog validates rules, and Scheme provides the AI. The system is designed to be easy to understand, extend, and debug.
