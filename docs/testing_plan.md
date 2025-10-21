# Testing Plan

## Unit Testing Strategy

### C++ Components

#### Board Class Tests
- [ ] Initialize board with correct size
- [ ] All cells start empty (0)
- [ ] getCell returns correct values
- [ ] setCell updates correctly
- [ ] isEmpty detects empty cells
- [ ] isFull detects full board
- [ ] Display shows correct format

#### Game Class Tests
- [ ] Game initializes with player 1
- [ ] switchPlayer alternates correctly
- [ ] makeMove rejects invalid positions
- [ ] makeMove rejects occupied cells
- [ ] Winner detection: row wins
- [ ] Winner detection: column wins
- [ ] Winner detection: diagonal wins
- [ ] Draw detection when board full

### Prolog Rules Tests

#### Valid Move Tests
```prolog
?- valid_move([[0,0,0],[0,0,0],[0,0,0]], 0, 0, 1).
true.

?- valid_move([[1,0,0],[0,0,0],[0,0,0]], 0, 0, 1).
false.

?- valid_move([[0,0,0],[0,0,0],[0,0,0]], 3, 3, 1).
false.
```

#### Winner Detection Tests
```prolog
?- check_winner([[1,1,1],[0,0,0],[0,0,0]], Winner).
Winner = 1.

?- check_winner([[1,0,0],[1,0,0],[1,0,0]], Winner).
Winner = 1.

?- check_winner([[1,0,0],[0,1,0],[0,0,1]], Winner).
Winner = 1.
```

### Scheme AI Tests

#### Valid Move Generation
- [ ] Returns only empty positions
- [ ] Returns at least one move when available
- [ ] Returns empty list when board full

#### Move Evaluation
- [ ] Recognizes immediate wins
- [ ] Blocks opponent's winning moves
- [ ] Prefers center/corners (strategy)

## Integration Testing

### Phase 1: C++ ↔ Prolog
- [ ] C++ writes valid move query
- [ ] Prolog reads query correctly
- [ ] Prolog validates and responds
- [ ] C++ reads response correctly
- [ ] Test with valid move
- [ ] Test with invalid move (occupied)
- [ ] Test with invalid move (out of bounds)

### Phase 2: C++ ↔ Scheme
- [ ] C++ writes board state
- [ ] Scheme reads state correctly
- [ ] Scheme returns valid move
- [ ] C++ applies AI move
- [ ] Test on empty board
- [ ] Test on partially filled board
- [ ] Test on nearly full board

### Phase 3: Full System
- [ ] Human vs Human game
- [ ] Human vs AI game
- [ ] AI makes legal moves only
- [ ] Game detects all win conditions
- [ ] Game detects draw correctly

## Manual Testing Scenarios

### Scenario 1: Complete Game
1. Start game
2. Player 1 moves to (0,0)
3. Player 2 moves to (1,1)
4. Player 1 moves to (0,1)
5. Player 2 moves to (2,2)
6. Player 1 moves to (0,2) → WIN
7. Verify game ends
8. Verify winner announced

### Scenario 2: Invalid Move Handling
1. Start game
2. Enter move (-1, 0) → rejected
3. Enter move (3, 3) → rejected
4. Enter move (0, 0) → accepted
5. Try (0, 0) again → rejected (occupied)

### Scenario 3: AI Gameplay
1. Start game with AI as player 2
2. Make move as player 1
3. Verify AI responds
4. Verify AI move is legal
5. Continue until game ends

### Scenario 4: Draw Detection
1. Play game to draw:
   ```
   X O X
   O O X
   O X O
   ```
2. Verify draw is detected
3. Verify game ends

## Performance Testing

### Benchmarks
- [ ] Board display: < 10ms
- [ ] Move validation: < 50ms (including Prolog call)
- [ ] AI move (3x3): < 500ms
- [ ] Complete game: < 2 minutes

## Debugging Checklist

When something goes wrong:
1. Check data/*.txt files for correct format
2. Test each component independently
3. Verify file permissions
4. Check for process spawning errors
5. Look for parsing errors in responses
6. Validate board state consistency

## Test Data Examples

### Empty Board State
```
3
0,0,0
0,0,0
0,0,0
1
```

### Mid-game State
```
3
1,0,2
0,1,0
2,0,0
1
```

### Near-win State (Player 1 can win)
```
3
1,1,0
2,2,0
0,0,0
1
```

