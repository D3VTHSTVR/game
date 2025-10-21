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
Board::Board(int size) : size(size) {
    // TODO: initialize grid with zeros (empty cells)
    // grid = vector<vector<int>>(size, vector<int>(size, 0));
}

/*
 * display - print board to console
 * parameters: none
 * returns: void
 */
void Board::display() const {
    // TODO: print top border
    // cout << "  ";
    // for (int i = 0; i < size; i++) {
    //     cout << i << " ";
    // }
    // cout << endl;
    
    // TODO: print each row with row number
    // for (int i = 0; i < size; i++) {
    //     cout << i << " ";
    //     for (int j = 0; j < size; j++) {
    //         if (grid[i][j] == 0) cout << ". ";
    //         else if (grid[i][j] == 1) cout << "X ";
    //         else cout << "O ";
    //     }
    //     cout << endl;
    // }
    
    cout << "Board display not yet implemented" << endl;
}

/*
 * getCell - get value at position
 * parameters: row, col - position to check
 * returns: int - cell value
 */
int Board::getCell(int row, int col) const {
    // TODO: validate bounds
    // if (row < 0 || row >= size || col < 0 || col >= size) {
    //     return -1;  // invalid position
    // }
    
    // TODO: return grid value
    // return grid[row][col];
    
    return 0; // placeholder
}

/*
 * setCell - set value at position
 * parameters: row, col - position to set, value - new cell value
 * returns: void
 */
void Board::setCell(int row, int col, int value) {
    // TODO: validate bounds
    // if (row < 0 || row >= size || col < 0 || col >= size) {
    //     return;
    // }
    
    // TODO: set grid value
    // grid[row][col] = value;
}

/*
 * isEmpty - check if cell is empty
 * parameters: row, col - position to check
 * returns: bool - true if cell is empty
 */
bool Board::isEmpty(int row, int col) const {
    // TODO: return true if cell value is 0
    // return getCell(row, col) == 0;
    
    return false; // placeholder
}

/*
 * isFull - check if board has no empty cells
 * parameters: none
 * returns: bool - true if board is full
 */
bool Board::isFull() const {
    // TODO: iterate through all cells
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         if (grid[i][j] == 0) return false;
    //     }
    // }
    // return true;
    
    return false; // placeholder
}

/*
 * getSize - get board dimension
 * parameters: none
 * returns: int - board size
 */
int Board::getSize() const {
    return size;
}

/*
 * reset - clear all cells
 * parameters: none
 * returns: void
 */
void Board::reset() {
    // TODO: set all cells to 0
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         grid[i][j] = 0;
    //     }
    // }
}

