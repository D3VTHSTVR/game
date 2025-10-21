# Quick Start Guide

## Installation

### 1. Install Dependencies

**macOS:**
```bash
# Install SWI-Prolog
brew install swi-prolog

# Install MIT Scheme
brew install mit-scheme

# C++ compiler (should already be installed)
xcode-select --install
```

**Linux:**
```bash
sudo apt-get install swi-prolog
sudo apt-get install mit-scheme
sudo apt-get install g++
```

### 2. Verify Installation

```bash
# Test C++ compiler
g++ --version

# Test Prolog
swipl --version

# Test Scheme
scheme --version
```

## Building the Project

```bash
cd /Users/devthstvr3/Desktop/CS396/Project

# Build the C++ game engine
make

# Clean build artifacts
make clean
```

## Running the Game

```bash
# After building
./game
```

## Development Workflow

### Working on C++
1. Edit files in `src/cpp/`
2. Run `make` to rebuild
3. Test with `./game`

### Working on Prolog
1. Edit `src/prolog/rules.pl`
2. Test interactively: `swipl -s src/prolog/rules.pl`
3. Try predicates: `?- valid_move(...).`

### Working on Scheme
1. Edit `src/scheme/ai.scm`
2. Test interactively: `scheme --load src/scheme/ai.scm`
3. Try functions: `(choose-best-move board 1)`

## Testing Individual Components

### Test C++ Board
```cpp
// Add to main.cpp temporarily
Board b(3);
b.display();
b.setCell(0, 0, 1);
b.display();
```

### Test Prolog Rules
```bash
swipl -s src/prolog/rules.pl
```
```prolog
?- valid_position(0, 0, 3).
true.

?- valid_position(5, 5, 3).
false.
```

### Test Scheme AI
```bash
scheme --load src/scheme/ai.scm
```
```scheme
(define test-board '((0 0 0) (0 1 0) (0 0 2)))
(get-valid-moves test-board)
```

## Debugging Tips

### Problem: "Command not found"
- Verify installations with `which swipl` and `which scheme`
- Add to PATH if needed

### Problem: Build errors
- Check that all header files are included
- Verify C++17 support: `g++ -std=c++17 --version`

### Problem: File I/O errors
- Check `data/` directory exists
- Verify file permissions
- Look at actual file contents: `cat data/current_state.txt`

### Problem: Prolog not responding
- Check Prolog syntax
- Run with trace: `?- trace, valid_move(...).`
- Exit Prolog: `?- halt.`

### Problem: Scheme errors
- Check parentheses matching
- Use simpler test cases first
- Debug with `(display ...)` statements

## Next Steps

1. **Implement Board class** - Start with display and basic operations
2. **Test file I/O** - Write and read simple test files
3. **Basic Prolog rules** - Get validation working
4. **Simple game loop** - Human vs human first
5. **Add Scheme AI** - Start with random moves
6. **Enhance AI** - Implement minimax

## Useful Commands

```bash
# Watch for file changes (macOS)
fswatch -o data/ | xargs -n1 -I{} cat data/current_state.txt

# View all data files
cat data/*.txt

# Quick rebuild and run
make clean && make && ./game

# Format C++ code (if clang-format installed)
clang-format -i src/cpp/*.cpp src/cpp/*.h
```

