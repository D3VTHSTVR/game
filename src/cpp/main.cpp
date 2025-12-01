/*
 * main.cpp
 * entry point for the cross-language board game system
 */

#include "game.h"
#include <iostream>
#include <limits>

using namespace std;

/* program entry point */
int main()
{
    // print header for the game
    cout << "=== Cross-Language Board Game System ===" << endl;
    cout << "Starting with Tic-Tac-Toe (Player 1 = Human, Player 2 = AI)" << endl;
    cout << endl;

    // create a tic tac toe game on a 3x3 board
    Game game(3);

    // run turns until game over
    while (!game.isGameOver())
    {
        // separate turns visually
        cout << endl;

        // display board and current player
        game.display();

        // branch based on whose turn it is
        if (game.getCurrentPlayer() == 1)
        {
            // human move inputs
            int row = -1;
            int col = -1;

            // prompt user for move
            cout << "Enter your move as 'row col': ";

            // validate input format
            if (!(cin >> row >> col))
            {
                // notify about invalid input
                cout << "Invalid input. Please enter two integers" << endl;

                // clear stream error state
                cin.clear();

                // discard the rest of the input line
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // retry turn
                continue;
            }

            // try applying the move
            if (!game.makeMove(row, col))
            {
                // move invalid or rejected
                cout << "Move rejected. Please try again" << endl;
                continue;
            }

            // notify user of accepted move
            cout << "Move accepted" << endl;
        }
        else
        {
            // scheme ai turn
            cout << "AI is thinking..." << endl;

            // get ai coordinates
            pair<int, int> aiMove = game.getAIMoveFromScheme();

            // validate ai output
            if (aiMove.first < 0 || aiMove.second < 0)
            {
                // ai failed to produce usable coordinates
                cout << "AI failed to provide a valid move. Exiting" << endl;
                return 1;
            }

            // show ai decision
            cout << "AI chooses: " << aiMove.first << ", " << aiMove.second << endl;

            // apply ai move
            if (!game.makeMove(aiMove.first, aiMove.second))
            {
                // ai generated an illegal move which should not happen
                cout << "AI move was rejected by the validator. Exiting" << endl;
                return 1;
            }
        }

        // advance to next player if game is not over
        if (!game.isGameOver())
        {
            game.switchPlayer();
        }
    }

    // print final board state
    cout << endl;
    game.display();

    // print game outcome
    switch (game.getWinner())
    {
    case -1:
        cout << "It's a draw" << endl;
        break;
    case 1:
        cout << "Player 1 wins" << endl;
        break;
    case 2:
        cout << "Player 2 (AI) wins" << endl;
        break;
    default:
        cout << "Game ended without a winner" << endl;
        break;
    }

    return 0;
}