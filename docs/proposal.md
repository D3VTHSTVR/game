# Project Proposal: Cross-Language Board Game System

**Team Members**: [Add names here]

**Course**: CS 396 — Principles of Languages (Fall 2025)

**Project Title**: Multi-Paradigm Board Game Engine with AI

## 1. Overview and Motivation

We propose a cross-language game system that demonstrates the strengths of three paradigms—imperative/OOP (C++), logic (Prolog), and functional (Scheme)—working together on a unified task: implementing and analyzing a two-player board game. We will deliver a playable Tic-Tac-Toe MVP, then, time permitting, extend concepts toward Checkers to highlight scalability of rules and AI.

The project emphasizes how languages can “speak” via a simple interoperability layer while each component remains idiomatic to its paradigm: C++ handles state and I/O, Prolog encodes rules declaratively, and Scheme implements strategy via search/evaluation.

## 2. System Architecture

- **C++ (engine)**: main loop, board state, user input, file-based IPC orchestration.
- **Prolog (rules)**: validates move legality and detects win/draw conditions.
- **Scheme (AI)**: generates moves (random → minimax) based on the current board.

All cross-language communication uses plain-text files in `data/`:
- `current_state.txt`: board size, rows as CSV, current player
- `move_query.txt`: `row,col,player`
- `move_response.txt`: either `valid`/`invalid` (from Prolog) or `row,col` (from Scheme)

Rationale for file-based IPC: language-agnostic, easy to debug, no FFI complexity, and sufficient for course-scale performance.

## 3. Language Components and Responsibilities

### 3.1 C++ — Game Engine (Imperative/OOP)
- Board representation (`Board` class: get/set, display, emptiness/full checks)
- Game controller (`Game` class: player turn, serialization to `data/`, invoking Prolog/Scheme via system calls)
- CLI interface for a quick, clear demo

Justification: C++ offers strong control over I/O and structured state, making it well-suited as the orchestrator and renderer.

### 3.2 Prolog — Rule Validator (Logic/Declarative)
- Predicates: `valid_position/3`, `is_empty/3`, `valid_move/4`
- Winner/draw detection: `check_winner/2`, `board_full/1`, `game_over/1`
- File-IO predicate to read board + query and emit `valid`/`invalid`

Justification: Legal-move and win rules are naturally expressed as constraints; Prolog’s backtracking makes rule exploration concise and robust.

### 3.3 Scheme — AI Player (Functional)
- Move generation: `get-valid-moves`
- Position evaluation: heuristics for immediate wins/blocks and positional value
- Search: random baseline → minimax (depth-limited)
- File-IO entry point to read state and write best move

Justification: Functional decomposition and recursion align well with search/evaluation; Scheme highlights abstraction and clarity for AI logic.

## 4. Data Representation and File Formats

Example Tic-Tac-Toe board (player 2 to move):
```
3
0,0,1
0,2,0
1,0,0
2
```
Move query: `1,1,2` → Prolog returns `valid` or `invalid`
AI response: `row,col` (e.g., `0,2`)

## 5. MVP Scope and Stretch Goals

### MVP (Week 13 progress check)
- Human vs. human Tic-Tac-Toe (C++ only) with full board operations
- Prolog-based move validation integrated into the C++ loop
- Scheme AI that can at least play random legal moves

### Stretch Goals (Week 15, if time permits)
- Scheme minimax with basic evaluation; optional alpha-beta pruning
- Enhanced C++ UX (move history, nicer board rendering)
- Early steps toward Checkers: 8×8 board representation and sample rules

## 6. Development Timeline and Milestones

- **Week 10 (Proposal)**: Finalize architecture and roles; stub code and build system in place
- **Week 11**: Complete `Board` and baseline `Game` loop; file I/O write/read; Prolog `valid_move/4`
- **Week 12**: Integrate Prolog end-to-end; Scheme random AI; human vs AI playable
- **Week 13 (Progress Check)**: Demo Tic-Tac-Toe with integrated validation + random AI
- **Week 14**: Improve AI (minimax), winner detection robustness, UX polish
- **Week 15 (Final Submission & Presentation)**: Final code, 3–4 page report, demo

Deliverables align with course milestones; we’ll maintain `docs/` and a clear `README.md` for build/run instructions.

## 7. Testing and Evaluation Plan

- Unit tests (manual/interactive) per component:
  - C++ `Board` operations (empty/full, set/get)
  - Prolog rule predicates (legal/illegal positions; winner detection)
  - Scheme AI (valid move generation; sanity checks on evaluation)
- Integration tests:
  - C++ ↔ Prolog: validate legal and illegal moves
  - C++ ↔ Scheme: ensure returned AI moves are legal and applied
- Performance targets (Tic-Tac-Toe):
  - Validation under 50ms, AI move under 500ms on typical dev hardware

## 8. Risks and Mitigations

- **IPC coordination complexity** → Keep formats minimal and human-readable; log intermediate files in `data/` during debugging
- **Learning curve for Scheme/Prolog** → Build simple baselines first (random AI, basic rules), iterate toward minimax and richer rules
- **Time constraints** → Lock down the Tic-Tac-Toe MVP early; treat Checkers as optional stretch

## 9. Roles and Collaboration

- C++ Lead: engine, serialization, CLI UX
- Prolog Lead: rules, validation, file I/O predicate
- Scheme Lead: AI algorithms (random → minimax), evaluation

All team members perform code reviews and integration testing. We will use feature branches (e.g., `vdev`) and PRs to integrate changes into `dev`.

## 10. Demo Plan (5 minutes)

1. 30s: System overview and architecture slide
2. 90s: Live demo — human vs AI; show validation and an AI response
3. 90s: Peek at code — Prolog rule snippet and Scheme evaluation function
4. 60s: Discuss paradigm benefits and trade-offs observed
5. 30s: Q&A

## 11. References and Setup

- Build & Run: see `README.md` and `docs/quick_start.md`
- Architecture details: `docs/architecture.md`
- Testing plan: `docs/testing_plan.md`

We will keep documentation up to date as features land, and ensure the final report reflects lessons learned about paradigm interoperability.

