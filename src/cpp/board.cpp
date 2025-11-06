/*
 * board.cpp
 * Implementation of board operations
 */

#include "board.h"
#include <iostream>

using namespace std;

/*
 * Board - constructor
 * parameters: size - dimension of board
 * returns: Board object
 */
/*
 * When a board is created we immediately allocate the nested vector structure
 * and reset every cell to zero. Using the constructor initializer ensures the
 * dimension is locked in before any other work happens.
 */
Board::Board(int size) : size(size)
{
    // Allocate the two-dimensional vector with every slot set to zero.
    grid = vector<vector<int>>(size, vector<int>(size, 0));
}

/*
 * display - print board to console
 * parameters: none
 * returns: void
 */
/*
 * Printing the board is handled here so the rest of the engine does not need
 * to know anything about the storage format. Column headers help human players
 * orient themselves, and the character mapping keeps the output compact.
 */
void Board::display() const
{
    cout << "  ";
    for (int i = 0; i < size; i++)
    {
        // Print each column index so a player can match coordinates to input.
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++)
    {
        cout << i << " ";
        for (int j = 0; j < size; j++)
        {
            // Decide which character to show based on the stored player token.
            if (grid[i][j] == 0)
            {
                cout << ". ";
            }
            else if (grid[i][j] == 1)
            {
                cout << "X ";
            }
            else
            {
                cout << "O ";
            }
        }
        cout << endl;
    }
}

/*
 * getCell - get value at position
 * parameters: row, col - position to check
 * returns: int - cell value
 */
/*
 * Accessing a cell funnels through this helper so we can centralize bounds
 * checking. Returning -1 for invalid coordinates gives callers a simple signal
 * that something went wrong without throwing an exception.
 */
int Board::getCell(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size)
    {
        // A negative return value signals the caller that the lookup failed.
        return -1;
    }
    return grid[row][col];
}

/*
 * setCell - set value at position
 * parameters: row, col - position to set, value - new cell value
 * returns: void
 */
/*
 * Setting a cell guards against out-of-range coordinates for safety. The
 * function intentionally does nothing when given invalid input because legal
 * move checking happens before this point.
 */
void Board::setCell(int row, int col, int value)
{
    if (row < 0 || row >= size || col < 0 || col >= size)
    {
        // Abort silently; higher layers already tell the player when a move
        // was illegal, and we do not want to corrupt memory here.
        return;
    }
    grid[row][col] = value;
}

/*
 * isEmpty - check if cell is empty
 * parameters: row, col - position to check
 * returns: bool - true if cell is empty
 */
/*
 * isEmpty is a small wrapper that reuses getCell so it automatically benefits
 * from the centralized bounds handling.
 */
bool Board::isEmpty(int row, int col) const
{
    return getCell(row, col) == 0;
}

/*
 * isFull - check if board has no empty cells
 * parameters: none
 * returns: bool - true if board is full
 */
/*
 * A board is considered full when no cell contains zero. The nested loops are
 * small for Tic-Tac-Toe but still written generically for future expansion.
 */
bool Board::isFull() const
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // The presence of any zero means at least one move remains.
            if (grid[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

/*
 * getSize - get board dimension
 * parameters: none
 * returns: int - board size
 */
/*
 * The size is exposed via a getter so we can keep the data member private while
 * still sharing the value with other subsystems.
 */
int Board::getSize() const
{
    return size;
}

/*
 * reset - clear all cells
 * parameters: none
 * returns: void
 */
/*
 * Calling reset walks the grid and clears every cell. This mirrors the work
 * done in the constructor and is useful if we add UI options to restart.
 */
void Board::reset()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Replace any player marker with the neutral empty value.
            grid[i][j] = 0;
        }
    }
}

