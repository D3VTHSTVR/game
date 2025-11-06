/*
 * main.cpp
 * Entry point for the cross-language board game system
 */

#include "game.h"
#include <iostream>
#include <limits>

using namespace std;

/*
 * main wires the user interface to the Game orchestrator. The terminal loop is
 * intentionally verbose so someone unfamiliar with the project can follow the
 * flow without stepping through a debugger.
 */
int main()
{
    cout << "=== Cross-Language Board Game System ===" << endl;
    cout << "Starting with Tic-Tac-Toe (Player 1 = Human, Player 2 = AI)" << endl;
    cout << endl;

    /*
     * The Game instance automatically prepares the shared data files.
     */
    Game game(3);

    /*
     * The loop runs until the Game object sets its gameOver flag. Inside the
     * loop we branch on the current player so we can either prompt a human or
     * delegate to the Scheme AI.
     */
    while (!game.isGameOver())
    {
        cout << endl;
        game.display();

        if (game.getCurrentPlayer() == 1)
        {
            int row = -1;
            int col = -1;

            cout << "Enter your move as 'row col': ";
            if (!(cin >> row >> col))
            {
                // Clear error flags and discard the bad line so we can prompt
                // the user again without getting stuck.
                cout << "Invalid input. Please enter two integers." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!game.makeMove(row, col))
            {
                // Prolog rejected the move (out of bounds or occupied cell).
                cout << "Move rejected. Please try again." << endl;
                continue;
            }

            cout << "Move accepted." << endl;
        }
        else
        {
            // Delegate the turn to the Scheme AI.
            cout << "AI is thinking..." << endl;
            pair<int, int> aiMove = game.getAIMoveFromScheme();
            if (aiMove.first < 0 || aiMove.second < 0)
            {
                // Scheme failed to produce a valid coordinate pair.
                cout << "AI failed to provide a valid move. Exiting." << endl;
                return 1;
            }

            cout << "AI chooses: " << aiMove.first << ", " << aiMove.second << endl;
            if (!game.makeMove(aiMove.first, aiMove.second))
            {
                // If the validator rejects the AI move, abort to avoid loops.
                cout << "AI move was rejected by the validator. Exiting." << endl;
                return 1;
            }
        }

        if (!game.isGameOver())
        {
            // Prepare for the next turn now that the current one resolved.
            game.switchPlayer();
        }
    }

    cout << endl;
    game.display();

    /*
     * Once the loop ends we inspect the winner flag to print the outcome.
     */
    switch (game.getWinner())
    {
        case -1:
            cout << "It's a draw!" << endl;
            break;
        case 1:
            cout << "Player 1 wins!" << endl;
            break;
        case 2:
            cout << "Player 2 (AI) wins!" << endl;
            break;
        default:
            cout << "Game ended without a winner." << endl;
            break;
    }

    return 0;
}

