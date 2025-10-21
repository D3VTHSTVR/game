# System Architecture

## Component Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                      C++ Game Engine                         │
│  ┌────────────┐  ┌────────────┐  ┌──────────────────────┐  │
│  │   main.cpp │  │  board.cpp │  │     game.cpp         │  │
│  │            │→ │  board.h   │← │     game.h           │  │
│  │  - game    │  │            │  │                      │  │
│  │    loop    │  │  - display │  │  - move validation   │  │
│  │  - user    │  │  - get/set │  │  - AI integration    │  │
│  │    input   │  │  - isEmpty │  │  - file I/O          │  │
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
    │                 │          │                  │
    │  rules.pl       │          │  ai.scm          │
    │                 │          │                  │
    │  - valid_move/4 │          │  - minimax       │
    │  - check_winner │          │  - evaluate      │
    │  - game_over    │          │  - best_move     │
    └────────┬────────┘          └────────┬─────────┘
             │ writes                     │ writes
             ↓                            ↓
    ┌──────────────────────────────────────────────┐
    │          move_response.txt                   │
    └────────────────┬─────────────────────────────┘
                     │ reads
                     ↓
             ┌───────────────┐
             │  C++ Engine   │
             │  (continues)  │
             └───────────────┘
```

## Data Flow

### 1. Human Move Flow
```
User Input → C++ validates format 
          → Writes move_query.txt 
          → Calls Prolog
          → Prolog validates rules
          → Writes move_response.txt
          → C++ reads response
          → If valid: update board
```

### 2. AI Move Flow
```
C++ detects AI turn → Writes current_state.txt
                    → Calls Scheme AI
                    → Scheme reads board
                    → Scheme computes best move
                    → Writes move_response.txt
                    → C++ reads and applies move
```

## File Formats

All files use simple text format for transparency and debugging.

### Board State Format
```
3           ← board size
0,0,1       ← row 0: empty, empty, player 1
0,2,0       ← row 1: empty, player 2, empty
1,0,0       ← row 2: player 1, empty, empty
2           ← current player
```

### Move Query Format
```
1,1,2       ← row=1, col=1, player=2
```

### Response Formats
```
valid       ← Prolog validation response
```
or
```
1,2         ← Scheme AI move (row=1, col=2)
```

## Design Decisions

### Why File-based Communication?
- **Simplicity**: No complex FFI or networking
- **Debugging**: Files can be inspected manually
- **Language Independence**: Works with any language
- **Portability**: No platform-specific IPC

### Why This Language Division?
- **C++ for state**: Efficient, familiar, good for I/O
- **Prolog for rules**: Declarative validation is natural
- **Scheme for AI**: Functional tree search algorithms

### Potential Improvements
- Replace file I/O with pipes for better performance
- Use JSON instead of CSV for structured data
- Embed interpreters directly in C++ (SWI-Prolog C interface, TinyScheme)
- Add threading for async AI computation

