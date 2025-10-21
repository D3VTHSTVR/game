/*
 * game.cpp
 * Implementation of game controller logic
 */

#include "game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

/*
 * Game - constructor
 * parameters: size - board dimension
 * returns: Game object
 */
Game::Game(int size) 
    : board(size), currentPlayer(1), gameOver(false), winner(0) {
    // TODO: initialize data directory if needed
    // TODO: clear any old communication files
}

/*
 * display - show current board state
 * parameters: none
 * returns: void
 */
void Game::display() const {
    board.display();
    cout << "Current player: " << currentPlayer << endl;
}

/*
 * makeMove - attempt to make a move
 * parameters: row, col - position to move to
 * returns: bool - true if move was valid and applied
 */
bool Game::makeMove(int row, int col) {
    // TODO: validate move with prolog
    // if (!validateMoveWithProlog(row, col)) {
    //     return false;
    // }
    
    // TODO: apply move to board
    // board.setCell(row, col, currentPlayer);
    
    // TODO: save updated state
    // saveBoardState();
    
    // TODO: check for winner
    // checkWinner();
    
    return true;
}

/*
 * validateMoveWithProlog - check if move is legal using Prolog
 * parameters: row, col - position to validate
 * returns: bool - true if move is legal
 */
bool Game::validateMoveWithProlog(int row, int col) {
    // TODO: save current board state to file
    // saveBoardState();
    
    // TODO: write move query to file
    // format: "row,col,player"
    // ofstream queryFile(QUERY_FILE);
    // queryFile << row << "," << col << "," << currentPlayer << endl;
    // queryFile.close();
    
    // TODO: call prolog validator
    // system("swipl -s src/prolog/rules.pl -g validate_move,halt");
    
    // TODO: read response from file
    // ifstream responseFile(RESPONSE_FILE);
    // string response;
    // getline(responseFile, response);
    // responseFile.close();
    
    // TODO: return true if response is "valid", false otherwise
    // return (response == "valid");
    
    return true; // placeholder
}

/*
 * getAIMoveFromScheme - request AI move from Scheme
 * parameters: none
 * returns: pair<int, int> - row and column of AI's chosen move
 */
pair<int, int> Game::getAIMoveFromScheme() {
    // TODO: save current board state
    // saveBoardState();
    
    // TODO: call scheme AI
    // system("scheme --quiet --load src/scheme/ai.scm < data/current_state.txt > data/move_response.txt");
    
    // TODO: read AI's chosen move from response file
    // format: "row,col"
    // ifstream responseFile(RESPONSE_FILE);
    // int row, col;
    // char comma;
    // responseFile >> row >> comma >> col;
    // responseFile.close();
    
    // TODO: return the move
    // return make_pair(row, col);
    
    return make_pair(0, 0); // placeholder
}

/*
 * isGameOver - check if game has ended
 * parameters: none
 * returns: bool - true if game is over
 */
bool Game::isGameOver() const {
    return gameOver;
}

/*
 * getWinner - get the winning player
 * parameters: none
 * returns: int - 0 = no winner yet, 1/2 = player, -1 = draw
 */
int Game::getWinner() const {
    return winner;
}

/*
 * switchPlayer - toggle current player
 * parameters: none
 * returns: void
 */
void Game::switchPlayer() {
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

/*
 * getCurrentPlayer - get current player number
 * parameters: none
 * returns: int - current player (1 or 2)
 */
int Game::getCurrentPlayer() const {
    return currentPlayer;
}

/*
 * saveBoardState - write current board to file for other languages
 * parameters: none
 * returns: void
 */
void Game::saveBoardState() {
    // TODO: open state file
    // ofstream stateFile(STATE_FILE);
    
    // TODO: write board size
    // stateFile << board.getSize() << endl;
    
    // TODO: write each cell value
    // for (int i = 0; i < board.getSize(); i++) {
    //     for (int j = 0; j < board.getSize(); j++) {
    //         stateFile << board.getCell(i, j);
    //         if (j < board.getSize() - 1) stateFile << ",";
    //     }
    //     stateFile << endl;
    // }
    
    // TODO: write current player
    // stateFile << currentPlayer << endl;
    
    // stateFile.close();
}

/*
 * checkWinner - determine if there's a winner
 * parameters: none
 * returns: void - updates winner and gameOver members
 */
void Game::checkWinner() {
    // TODO: check rows for three in a row
    // for (int row = 0; row < board.getSize(); row++) {
    //     check if all cells in row are same and non-zero
    // }
    
    // TODO: check columns for three in a row
    // for (int col = 0; col < board.getSize(); col++) {
    //     check if all cells in column are same and non-zero
    // }
    
    // TODO: check diagonals for three in a row
    // check top-left to bottom-right
    // check top-right to bottom-left
    
    // TODO: check for draw (board full, no winner)
    // if (board.isFull() && winner == 0) {
    //     winner = -1;
    //     gameOver = true;
    // }
}

