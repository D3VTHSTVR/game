/*
 * game.h
 * Game controller - manages game flow and inter-language communication
 */

#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <string>
#include <utility>

/*
 * Game is the high level orchestrator. It owns the board, keeps track of whose
 * turn it is, and is responsible for forwarding information to Prolog and
 * Scheme through the shared text files described in the project proposal.
 */
class Game
{
private:
    /*
     * board encapsulates the actual grid state.
     */
    Board board;

    /*
     * currentPlayer stores which side is about to act. The value is either 1 or
     * 2 and the number maps directly to the markers X and O when rendering.
     */
    int currentPlayer;

    /*
     * gameOver is flipped to true once Prolog or internal checks find a win or
     * draw. The flag lets the outer loop stop cleanly.
     */
    bool gameOver;

    /*
     * winner communicates the outcome back to the UI. A value of zero means the
     * game is still in progress, positive numbers identify the player, and -1
     * stands for a draw after the board fills up.
     */
    int winner;

    /*
     * These constants define the files used to shuttle data between the three
     * languages. The Game class is the only place that should know the actual
     * filenames so the contract stays consistent.
     */
    const std::string STATE_FILE = "data/current_state.txt";
    const std::string QUERY_FILE = "data/move_query.txt";
    const std::string RESPONSE_FILE = "data/move_response.txt";

public:
    /*
     * Game - constructor
     * parameters: size - board dimension (3 for tic-tac-toe, 8 for checkers)
     */
    Game(int size);

    /*
     * display - show current board state
     * parameters: none
     * returns: void
     */
    void display() const;

    /*
     * makeMove - attempt to make a move
     * parameters: row, col - position to move to
     * returns: bool - true if move was valid and applied
     */
    bool makeMove(int row, int col);

    /*
     * validateMoveWithProlog - check if move is legal using Prolog
     * parameters: row, col - position to validate
     * returns: bool - true if move is legal
     */
    bool validateMoveWithProlog(int row, int col);

    /*
     * getAIMoveFromScheme - request AI move from Scheme
     * parameters: none
     * returns: pair<int, int> - row and column of AI's chosen move
     */
    std::pair<int, int> getAIMoveFromScheme();

    /*
     * isGameOver - check if game has ended
     * parameters: none
     * returns: bool - true if game is over
     */
    bool isGameOver() const;

    /*
     * getWinner - get the winning player
     * parameters: none
     * returns: int - 0 = no winner yet, 1/2 = player, -1 = draw
     */
    int getWinner() const;

    /*
     * switchPlayer - toggle current player
     * parameters: none
     * returns: void
     */
    void switchPlayer();

    /*
     * getCurrentPlayer - get current player number
     * parameters: none
     * returns: int - current player (1 or 2)
     */
    int getCurrentPlayer() const;

private:
    /*
     * saveBoardState - write current board to file for other languages
     * parameters: none
     * returns: void
     */
    void saveBoardState();

    /*
     * checkWinner - determine if there's a winner
     * parameters: none
     * returns: void - updates winner and gameOver members
     */
    void checkWinner();
}
;

#endif // GAME_H

