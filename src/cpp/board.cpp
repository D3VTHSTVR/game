#include "board.h"
#include <iostream>

using namespace std;

/* board constructor */
Board::Board(int size) : size(size)
{
    // create a size x size grid filled with zeros
    grid = vector<vector<int>>(size, vector<int>(size, 0));
}

/* display the board */
void Board::display() const
{
    // print column headers
    cout << "  ";
    for (int i = 0; i < size; i++)
    {
        // print each column index
        cout << i << " ";
    }
    cout << endl;

    // loop through each row
    for (int i = 0; i < size; i++)
    {
        // print the row index
        cout << i << " ";

        // loop through each column in the row
        for (int j = 0; j < size; j++)
        {
            // print '.' for empty, 'X' for player 1, 'O' for player 2
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

        // end the line after the row
        cout << endl;
    }
}

/* get value stored at a cell */
int Board::getCell(int row, int col) const
{
    // check for out of bounds access
    if (row < 0 || row >= size || col < 0 || col >= size)
    {
        // return -1 to signal invalid lookup
        return -1;
    }

    // return the value at the requested cell
    return grid[row][col];
}

/* set a cell value */
void Board::setCell(int row, int col, int value)
{
    // ignore invalid coordinates
    if (row < 0 || row >= size || col < 0 || col >= size)
    {
        return;
    }

    // update the cell
    grid[row][col] = value;
}

/* check if a cell is empty */
bool Board::isEmpty(int row, int col) const
{
    // a cell is empty when its value is zero
    return getCell(row, col) == 0;
}

/* check if the board is full */
bool Board::isFull() const
{
    // loop through each cell in the board
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // if any cell is zero the board is not full
            if (grid[i][j] == 0)
            {
                return false;
            }
        }
    }

    // no empty cells found
    return true;
}

/* get the size of the board */
int Board::getSize() const
{
    // return the dimension of the board
    return size;
}

/* clear the entire board */
void Board::reset()
{
    // loop through each cell and set to zero
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            grid[i][j] = 0;
        }
    }
}