# Implementation Checklist

Use this checklist to track your progress through the project.

## Phase 1: Basic Infrastructure (Week 10-11)

### C++ Board Class
- [ ] Initialize grid with proper size
- [ ] Implement `display()` method
- [ ] Implement `getCell()` and `setCell()`
- [ ] Implement `isEmpty()` check
- [ ] Implement `isFull()` check
- [ ] Implement `reset()` method
- [ ] Test all board operations

### C++ Game Class - Basic Setup
- [ ] Initialize board in constructor
- [ ] Implement basic `display()` wrapper
- [ ] Implement `switchPlayer()`
- [ ] Implement `getCurrentPlayer()`
- [ ] Create `data/` directory if not exists
- [ ] Implement `saveBoardState()` to file

### File I/O Setup
- [ ] Create test board state file manually
- [ ] Test reading board state file
- [ ] Test writing board state file
- [ ] Verify file format is correct

### Basic Game Loop
- [ ] Accept user input for row/col
- [ ] Validate input format
- [ ] Apply move to board
- [ ] Switch players
- [ ] Display updated board
- [ ] Test human vs human gameplay

## Phase 2: Prolog Integration (Week 11-12)

### Prolog Rules - Basic
- [ ] Define board representation
- [ ] Implement `valid_position/3`
- [ ] Implement `is_empty/3`
- [ ] Test predicates interactively

### Prolog Rules - Move Validation
- [ ] Implement `valid_move/4`
- [ ] Read move query from file
- [ ] Write validation response to file
- [ ] Test with various move queries

### C++ ↔ Prolog Integration
- [ ] Implement `validateMoveWithProlog()` in C++
- [ ] Write move query to file
- [ ] Call swipl from C++ using system()
- [ ] Read Prolog response
- [ ] Parse response (valid/invalid)
- [ ] Integrate into game loop
- [ ] Test end-to-end validation

### Prolog Rules - Game State
- [ ] Implement `check_line/2` for three-in-a-row
- [ ] Implement `check_winner/2`
- [ ] Implement `board_full/1`
- [ ] Implement `game_over/1`
- [ ] Test all win conditions

### C++ Winner Detection
- [ ] Implement `checkWinner()` method
- [ ] Call after each move
- [ ] Update `gameOver` flag
- [ ] Update `winner` variable
- [ ] Test all win scenarios

## Phase 3: Scheme AI (Week 12-13)

### Scheme AI - Basic Structure
- [ ] Define board representation
- [ ] Implement `read-board-state`
- [ ] Test reading from file
- [ ] Parse into Scheme list structure

### Scheme AI - Move Generation
- [ ] Implement `get-valid-moves`
- [ ] Test on various boards
- [ ] Implement `choose-random-move`
- [ ] Test random move selection

### C++ ↔ Scheme Integration
- [ ] Implement `getAIMoveFromScheme()` in C++
- [ ] Save board state to file
- [ ] Call scheme from C++ using system()
- [ ] Read AI move response
- [ ] Parse move coordinates
- [ ] Test end-to-end AI move

### Scheme AI - Evaluation
- [ ] Implement `evaluate-position`
- [ ] Check for wins (+100)
- [ ] Check for losses (-100)
- [ ] Check for two-in-a-row opportunities
- [ ] Test evaluation function

### Scheme AI - Minimax
- [ ] Implement basic minimax skeleton
- [ ] Add depth limiting
- [ ] Add maximizing player logic
- [ ] Add minimizing player logic
- [ ] Integrate evaluation function
- [ ] Test on various board states

### Scheme AI - Best Move
- [ ] Implement `choose-best-move`
- [ ] Try all valid moves
- [ ] Score each with minimax
- [ ] Return highest scoring move
- [ ] Test against human player

## Phase 4: Polish & Enhancement (Week 14-15)

### Game Loop Enhancements
- [ ] Add player type selection (human/AI)
- [ ] Add difficulty levels (random/minimax)
- [ ] Add move history display
- [ ] Add move undo (optional)
- [ ] Improve board display formatting

### AI Improvements
- [ ] Add alpha-beta pruning (optional)
- [ ] Tune evaluation weights
- [ ] Add opening book (optional)
- [ ] Improve move selection for draws

### Error Handling
- [ ] Handle file I/O errors gracefully
- [ ] Handle invalid user input
- [ ] Handle Prolog/Scheme execution errors
- [ ] Add helpful error messages

### Testing
- [ ] Complete all unit tests
- [ ] Run all integration tests
- [ ] Play test complete games
- [ ] Test edge cases (full board, etc.)
- [ ] Verify all win conditions detected

### Documentation
- [ ] Complete proposal.md with team info
- [ ] Add code comments throughout
- [ ] Document build process in README
- [ ] Create demo script
- [ ] Prepare presentation slides

### Optional: Scale to Checkers
- [ ] Design 8x8 board representation
- [ ] Implement piece movement rules
- [ ] Implement jump logic
- [ ] Implement king promotion
- [ ] Update Prolog rules for checkers
- [ ] Update AI evaluation for checkers

## Deliverables Checklist

### Week 10: Proposal
- [ ] Team members listed
- [ ] Problem description clear
- [ ] Languages and roles defined
- [ ] Communication method explained
- [ ] Timeline included

### Week 13: Progress Check
- [ ] Working tic-tac-toe demo
- [ ] Human vs human working
- [ ] Basic AI moves (random or better)
- [ ] Code walkthrough prepared

### Week 15: Final Submission
- [ ] All source code complete
- [ ] 3-4 page report written
- [ ] README with build instructions
- [ ] Code well-commented
- [ ] All components tested

### Week 15: Presentation
- [ ] 5-minute demo prepared
- [ ] Architecture diagram ready
- [ ] Example gameplay recorded
- [ ] Paradigm comparison prepared
- [ ] Q&A preparation done

