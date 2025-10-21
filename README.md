# Cross-Language Board Game System

A two-player board game (Tic-Tac-Toe → Checkers) demonstrating paradigm integration:
- **C++**: Game engine, state management, I/O
- **Prolog**: Rule validation and move legality checking
- **Scheme**: AI strategy and decision making

## Project Structure

```
Project/
├── src/
│   ├── cpp/          # C++ game engine
│   ├── scheme/       # Scheme AI player
│   └── prolog/       # Prolog rule validator
├── data/             # Inter-process communication files
├── docs/             # Documentation
└── Makefile          # Build system
```

## Setup Instructions

### Prerequisites
- C++ compiler (g++ or clang++)
- SWI-Prolog (`brew install swi-prolog` on macOS)
- MIT Scheme or Racket (`brew install mit-scheme` on macOS)

### Build and Run

```bash
# Build the C++ game engine
make

# Run the game
./game

# Test individual components
make test-prolog
make test-scheme
```

## Development Phases

### Phase 1: Tic-Tac-Toe (Proof of Concept)
- Basic 3x3 board
- Simple rule validation
- Random AI moves

### Phase 2: Enhanced AI
- Minimax algorithm in Scheme
- Difficulty levels

### Phase 3: Scale to Checkers
- 8x8 board
- Jump logic
- King pieces

## Team Members
- Member 1: C++ game engine
- Member 2: Scheme AI
- Member 3: Prolog rules

## Communication Protocol

Components communicate via simple text files in `data/`:
- `current_state.txt`: Current board state
- `move_query.txt`: Proposed move to validate
- `move_response.txt`: Validation result or AI move

