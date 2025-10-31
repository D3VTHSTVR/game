# Proposal

Team Members: Victor Rodriguez, Taylor Davis, Anthony

Course: CS 396 — SEC001 — Fall 2025

## Project Title and Summary

**Project Title**: Cross-Language Board Game System

We’re building a small two‑player board game (Tic‑Tac‑Toe to start) that demonstrates how three different programming paradigms—imperative/OOP (C++), logic (Prolog), and functional (Scheme)—can integrate to solve a unified problem. C++ runs the main loop and keeps the board state, Prolog checks if moves are legal (and can detect wins), and Scheme picks AI moves. The parts will talk to each other using simple text files so it’s easy to test and debug. If we have time, we’ll try to extend some ideas toward Checkers.

## Languages Used and Roles
- C++ (Imperative/OOP): Board representation (`Board`), controller (`Game`), CLI, file I/O to `data/`.
- Prolog (Logic): Predicates for `valid_move/4`, `check_winner/2`, `game_over/1`; reads board/move, returns `valid`/`invalid`.
- Scheme (Functional): Move generation (`get-valid-moves`), evaluation, and search (random → depth-limited minimax); outputs `row,col`.

## Planned Integration Method
- File-based interchange in `data/` (language-agnostic, human-readable):
  - `current_state.txt`: size, rows as CSV, current player
  - `move_query.txt`: `row,col,player`
  - `move_response.txt`: `valid|invalid` (Prolog) or `row,col` (Scheme)
- C++ orchestrates: writes state/query → calls Prolog/Scheme → reads response.

### Architecture Diagram (Inline)
```
┌─────────────────────────────────────────────────────────────┐
│                      C++ Game Engine                         │
│  ┌────────────┐  ┌────────────┐  ┌──────────────────────┐  │
│  │   main.cpp │  │  board.cpp │  │     game.cpp         │  │
│  │            │→ │  board.h   │← │     game.h           │  │
│  │  - loop    │  │  - display │  │  - validation bridge │  │
│  │  - I/O     │  │  - get/set │  │  - AI integration    │  │
│  └────────────┘  └────────────┘  └──────────────────────┘  │
└────────────┬──────────────────────────────┬─────────────────┘
             │                              │
             │ writes                       │ writes
             ↓                              ↓
    ┌─────────────────┐          ┌──────────────────┐
    │ move_query.txt  │          │ current_state.txt│
    └────────┬────────┘          └────────┬─────────┘
             │                            │
             │ reads                      │ reads
             ↓                            ↓
    ┌─────────────────┐          ┌──────────────────┐
    │ Prolog Validator│          │  Scheme AI       │
    │  - valid_move   │          │  - minimax       │
    │  - winner check │          │  - evaluate      │
    └────────┬────────┘          └────────┬─────────┘
             │ writes                     │ writes
             ↓                            ↓
    ┌──────────────────────────────────────────────┐
    │              move_response.txt               │
    └────────────────┬─────────────────────────────┘
                     │ reads
                     ↓
                C++ Engine
```

### Data Flow (Inline)
```
Human Move:
User input → C++ formats query → write move_query.txt → run Prolog →
Prolog validates → write move_response.txt → C++ applies if valid

AI Move:
C++ writes current_state.txt → run Scheme AI → Scheme computes move →
write move_response.txt → C++ applies move
```

## Technical Sketch

**C++ Modules:**
- `class Board`: display, getCell/setCell, isEmpty/isFull, reset
- `class Game`: currentPlayer, saveBoardState(), validateMoveWithProlog(), getAIMoveFromScheme(), checkWinner()
- `main()`: game loop (print board, read input, invoke Prolog/Scheme, switch players)

**Prolog Predicates:**
- Basic checks: `valid_position/3`, `is_empty/3`, `valid_move/4`
- Game state: `check_winner/2`, `board_full/1`, `game_over/1`
- Entry point: `validate_move` (reads files → decides → writes `valid|invalid`)

**Scheme Functions:**
- I/O: `read-board-state`
- Strategy: `get-valid-moves`, `evaluate-position`, `minimax`, `choose-best-move`

## Anticipated Challenges
- Getting data to line up across languages. We’ll keep the file formats super simple (CSV lines) and add basic checks.
- Spawning processes and reading files in the right order. We’ll call tools synchronously from C++ and keep the file flow clear.
- Writing the win/validation rules correctly. We’ll test small cases in Prolog first.
- AI complexity. We’ll start with random moves and add a depth‑limited minimax only if time allows.

Tools we plan to use: C++17, SWI‑Prolog, and MIT Scheme on macOS. If someone is on Windows, we’ll install the Windows versions of those tools or use WSL.
