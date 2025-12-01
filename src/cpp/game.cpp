/*
 * game.cpp
 * implementation of game controller logic
 */

#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <system_error>

using namespace std;

/* create a new game and initialize board state */
Game::Game(int size)
    : board(size), currentPlayer(1), gameOver(false), winner(0)
{
    // create data directory if missing
    namespace fs = std::filesystem;
    const fs::path dataDir("data");
    std::error_code ec;
    fs::create_directories(dataDir, ec);

    // save initial board state
    saveBoardState();
}

/* display the board and current player */
void Game::display() const
{
    // show board contents
    board.display();

    // print current player's turn
    cout << "Current player: " << currentPlayer << endl;
}

/* attempt to make a move */
bool Game::makeMove(int row, int col)
{
    // validate the move using prolog
    if (!validateMoveWithProlog(row, col))
    {
        return false;
    }

    // apply the move to the board
    board.setCell(row, col, currentPlayer);

    // update board file
    saveBoardState();

    // check if move ends the game
    checkWinner();

    // move was accepted
    return true;
}

/* validate a move through prolog rules */
bool Game::validateMoveWithProlog(int row, int col)
{
    // save board so prolog reads accurate data
    saveBoardState();

    // write the move query
    {
        ofstream queryFile(QUERY_FILE, ios::trunc);

        // fail if write is impossible
        if (!queryFile)
        {
            cerr << "Unable to write move query file" << endl;
            return false;
        }

        // format is row,col,player
        queryFile << row << ',' << col << ',' << currentPlayer << endl;
        queryFile.flush();
    }

    // remove old response file
    std::error_code ec;
    std::filesystem::remove(RESPONSE_FILE, ec);

    // call prolog to validate
    const int exitCode =
        system("swipl -q -s src/prolog/rules.pl -g validate_move -t halt");

    // check prolog exit status
    if (exitCode != 0)
    {
        cerr << "Prolog validator exited with code " << exitCode << endl;
        return false;
    }

    // open the prolog response file
    ifstream responseFile(RESPONSE_FILE);
    if (!responseFile)
    {
        cerr << "Unable to read move response from Prolog" << endl;
        return false;
    }

    // read the result
    string response;
    responseFile >> response;

    // valid only if prolog outputs the word "valid"
    return response == "valid";
}

/* request the ai move from racket scheme */
pair<int, int> Game::getAIMoveFromScheme()
{
    // save board so the scheme ai sees latest state
    saveBoardState();

    // remove old scheme output
    std::error_code ec;
    std::filesystem::remove(RESPONSE_FILE, ec);

    // run ai script
    const int exitCode = system("racket src/scheme/ai.rkt");

    // ensure scheme executed correctly
    if (exitCode != 0)
    {
        cerr << "Scheme AI exited with code " << exitCode << endl;
        return make_pair(-1, -1);
    }

    // open response file
    ifstream responseFile(RESPONSE_FILE);
    if (!responseFile)
    {
        cerr << "Unable to read Scheme AI response" << endl;
        return make_pair(-1, -1);
    }

    // read the raw "row,col" response
    string line;
    getline(responseFile, line);

    // parse the coordinates
    stringstream ss(line);
    int row = -1;
    int col = -1;
    char comma = ',';

    // parse format row,col
    if (!(ss >> row))
        return make_pair(-1, -1);
    if (!(ss >> comma) || comma != ',')
        return make_pair(-1, -1);
    if (!(ss >> col))
        return make_pair(-1, -1);

    // return parsed coordinates
    return make_pair(row, col);
}

/* return whether game is over */
bool Game::isGameOver() const
{
    return gameOver;
}

/* return winner value */
int Game::getWinner() const
{
    return winner;
}

/* switch active player */
void Game::switchPlayer()
{
    // toggle between players 1 and 2
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

/* return current player */
int Game::getCurrentPlayer() const
{
    return currentPlayer;
}

/* write the board state to the shared file */
void Game::saveBoardState()
{
    // open output file
    ofstream stateFile(STATE_FILE, ios::trunc);

    // ensure writable
    if (!stateFile)
    {
        cerr << "Unable to write board state file" << endl;
        return;
    }

    // write board size
    const int size = board.getSize();
    stateFile << size << endl;

    // write each row of the grid
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            // write cell value
            stateFile << board.getCell(row, col);

            // add comma except at the last entry
            if (col < size - 1)
            {
                stateFile << ',';
            }
        }

        // end the row
        stateFile << endl;
    }

    // write whose turn it is
    stateFile << currentPlayer << endl;

    // flush data to disk
    stateFile.flush();
}

/* check for a winner or draw */
void Game::checkWinner()
{
    // reset winner state
    winner = 0;
    gameOver = false;

    // get board size
    const int size = board.getSize();

    // check each row
    for (int row = 0; row < size; ++row)
    {
        const int value = board.getCell(row, 0);

        // skip empty row starts
        if (value == 0)
            continue;

        bool same = true;

        // verify row is uniform with natural loop stopping condition
        for (int col = 1; col < size && same; ++col)
        {
            if (board.getCell(row, col) != value)
            {
                same = false;
            }
        }

        // winning row found
        if (same)
        {
            winner = value;
            gameOver = true;
            return;
        }
    }

    // check each column
    for (int col = 0; col < size; ++col)
    {
        const int value = board.getCell(0, col);

        // skip empty column starts
        if (value == 0)
            continue;

        bool same = true;

        // verify column is uniform
        for (int row = 1; row < size && same; ++row)
        {
            if (board.getCell(row, col) != value)
            {
                same = false;
            }
        }

        // winning column found
        if (same)
        {
            winner = value;
            gameOver = true;
            return;
        }
    }

    // check main diagonal
    int diagValue = board.getCell(0, 0);
    if (diagValue != 0)
    {
        bool same = true;

        // verify diagonal
        for (int i = 1; i < size && same; ++i)
        {
            if (board.getCell(i, i) != diagValue)
            {
                same = false;
            }
        }

        // winning diagonal found
        if (same)
        {
            winner = diagValue;
            gameOver = true;
            return;
        }
    }

    // check anti diagonal
    diagValue = board.getCell(0, size - 1);
    if (diagValue != 0)
    {
        bool same = true;

        // verify anti diagonal
        for (int i = 1; i < size && same; ++i)
        {
            if (board.getCell(i, size - 1 - i) != diagValue)
            {
                same = false;
            }
        }

        // winning anti diagonal
        if (same)
        {
            winner = diagValue;
            gameOver = true;
            return;
        }
    }

    // detect draw if board is full
    if (board.isFull())
    {
        winner = -1;
        gameOver = true;
    }
}