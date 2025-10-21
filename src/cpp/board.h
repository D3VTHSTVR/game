/*
 * board.h
 * Board representation and basic operations
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
    int size;                              // board dimension (3x3 or 8x8)
    std::vector<std::vector<int>> grid;    // 0 = empty, 1 = player 1, 2 = player 2

public:
    /*
     * Board - constructor
     * parameters: size - dimension of board (3 for tic-tac-toe)
     * returns: Board object
     */
    Board(int size);
    
    /*
     * display - print board to console
     * parameters: none
     * returns: void
     */
    void display() const;
    
    /*
     * getCell - get value at position
     * parameters: row, col - position to check
     * returns: int - cell value (0, 1, or 2)
     */
    int getCell(int row, int col) const;
    
    /*
     * setCell - set value at position
     * parameters: row, col - position to set, value - new cell value
     * returns: void
     */
    void setCell(int row, int col, int value);
    
    /*
     * isEmpty - check if cell is empty
     * parameters: row, col - position to check
     * returns: bool - true if cell is empty
     */
    bool isEmpty(int row, int col) const;
    
    /*
     * isFull - check if board has no empty cells
     * parameters: none
     * returns: bool - true if board is full
     */
    bool isFull() const;
    
    /*
     * getSize - get board dimension
     * parameters: none
     * returns: int - board size
     */
    int getSize() const;
    
    /*
     * reset - clear all cells
     * parameters: none
     * returns: void
     */
    void reset();
};

#endif // BOARD_H

