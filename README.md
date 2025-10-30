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

See `dependencies.txt` for tested tool versions and install notes.

### Build and Run

```bash
# Build the C++ game engine
make

# Run the game
./game

# Note: current executable runs a temporary board display test until
# Prolog/Scheme integration is added.

# Test individual components
make test-prolog
make test-scheme
```

### Windows Setup Notes

- Recommended: Use Windows Subsystem for Linux (WSL) for the smoothest experience.
  - Install WSL, then inside Ubuntu/Debian:
    ```bash
    sudo apt-get update
    sudo apt-get install g++ make swi-prolog mit-scheme
    ```
  - Build/run the same as macOS/Linux: `make && ./game`

- Native Windows (without WSL):
  - Install a C++ toolchain (e.g., MSYS2 MinGW). Use the MSYS2 MinGW shell and run:
    ```bash
    pacman -S --needed mingw-w64-x86_64-gcc make
    ```
  - Install SWI-Prolog and MIT Scheme for Windows via their installers.
  - Use `make` from the MSYS2 environment. Running `./game` works in that shell.

- Notes:
  - Paths and line endings: file I/O uses plain text; parsing tolerates `\n`/`\r\n`.
  - External calls: if `swipl` or `scheme` aren’t on PATH, set full paths or use WSL.

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

