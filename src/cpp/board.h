/*
 * board.h
 * Board representation and basic operations
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>

/*
 * The Board class owns the raw grid of the game.
 * Each cell stores an integer: 0 means empty, 1 is player one, 2 is player two.
 * The class exposes utility helpers so the rest of the system never has to
 * manipulate the two dimensional std::vector directly.
 */
class Board
{
private:
    /*
     * size tracks the width and height of the square grid.
     * For Tic-Tac-Toe the value is 3; the design allows future expansion to
     * larger games such as Checkers by changing this single number.
     */
    int size;

    /*
     * grid stores rows of integers in a nested std::vector.
     * Access is always routed through helper functions so that bounds checking
     * and other invariants stay in one place.
     */
    std::vector<std::vector<int>> grid;

public:
    /*
     * Construct a board with all cells cleared to zero.
     */
    Board(int size);

    /*
     * Render the board to stdout using friendly characters so humans can see
     * the current state of play.
     */
    void display() const;

    /*
     * Return the player token stored at the requested row and column.
     * Out-of-range coordinates return -1 to signal an invalid lookup.
     */
    int getCell(int row, int col) const;

    /*
     * Write a player token into a cell as long as the coordinates are valid.
     * Invalid coordinates are silently ignored because validation happens
     * earlier in the flow.
     */
    void setCell(int row, int col, int value);

    /*
     * Convenience helper that returns true when a cell contains zero.
     */
    bool isEmpty(int row, int col) const;

    /*
     * Scan the entire grid to see if the game has any moves left.
     */
    bool isFull() const;

    /*
     * Expose the configured board dimension for callers that need it.
     */
    int getSize() const;

    /*
     * Reset every cell back to the empty state in one pass.
     */
    void reset();
}
;

#endif // BOARD_H

