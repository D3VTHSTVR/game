/*
 * board.h
 * board representation and basic operations
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>

/* board class stores the grid and provides helper functions */
class Board
{
private:
    /* size of the board */
    int size;

    /* 2d vector storing the board cells */
    std::vector<std::vector<int>> grid;

public:
    /* create a board with all cells set to zero */
    Board(int size);

    /* print the board for the user */
    void display() const;

    /* get the value of a cell or -1 if invalid */
    int getCell(int row, int col) const;

    /* set a cell value if the coordinates are valid */
    void setCell(int row, int col, int value);

    /* check if a cell is empty */
    bool isEmpty(int row, int col) const;

    /* check if the board has any empty cells left */
    bool isFull() const;

    /* get the board dimension */
    int getSize() const;

    /* clear all cells back to zero */
    void reset();
};

#endif // BOARD_H