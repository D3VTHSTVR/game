/*
 * game.cpp
 * Implementation of game controller logic
 */

#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <system_error>

using namespace std;

/*
 * Game - constructor
 * parameters: size - board dimension
 * returns: Game object
 */
/*
 * The constructor establishes the initial turn order and ensures that the file
 * based communication channel exists. Any stale data is cleared so the first
 * move always starts from a known state.
 */
Game::Game(int size)
    : board(size), currentPlayer(1), gameOver(false), winner(0)
{
    namespace fs = std::filesystem;
    const fs::path dataDir("data");
    std::error_code ec;
    // Ensure the shared data directory exists on disk.
    fs::create_directories(dataDir, ec);

    // Wipe out any stale state so a fresh match begins from a blank slate.
    std::ofstream stateClear(STATE_FILE);
    std::ofstream queryClear(QUERY_FILE);
    std::ofstream responseClear(RESPONSE_FILE);

    if (!stateClear || !queryClear || !responseClear)
    {
        std::cerr << "Failed to initialise data files in " << dataDir << std::endl;
    }

    // Persist the initial empty board for downstream consumers.
    saveBoardState();
}

/*
 * display - show current board state
 * parameters: none
 * returns: void
 */
/*
 * Showing the current board and player each turn keeps the terminal interface
 * approachable for people walking up to the game with no context.
 */
void Game::display() const
{
    board.display();
    cout << "Current player: " << currentPlayer << endl;
}

/*
 * makeMove - attempt to make a move
 * parameters: row, col - position to move to
 * returns: bool - true if move was valid and applied
 */
/*
 * makeMove is the single entry point for applying a human or AI action. The
 * function delegates the heavy lifting to Prolog so that rule enforcement lives
 * in exactly one place. Only when Prolog agrees do we touch the board.
 */
bool Game::makeMove(int row, int col)
{
    // Ask Prolog to confirm the move before altering local state.
    if (!validateMoveWithProlog(row, col))
    {
        return false;
    }

    // Record the move on the board so the UI and win checker see it.
    board.setCell(row, col, currentPlayer);
    // Mirror the updated board in the shared file format.
    saveBoardState();
    // Evaluate whether the new move ends the game.
    checkWinner();

    return true;
}

/*
 * validateMoveWithProlog - check if move is legal using Prolog
 * parameters: row, col - position to validate
 * returns: bool - true if move is legal
 */
/*
 * Every validation round mirrors the contract documented in docs/data/README.
 * 1) Persist the latest board so Prolog can reconstruct it.
 * 2) Write the candidate move into move_query.txt.
 * 3) Invoke SWI-Prolog in batch mode to run validate_move.
 * 4) Read the answer and return true only when Prolog writes "valid".
 */
bool Game::validateMoveWithProlog(int row, int col)
{
    // Step one: sync the board state so Prolog reads an accurate snapshot.
    saveBoardState();

    {
        ofstream queryFile(QUERY_FILE);
        if (!queryFile)
        {
            cerr << "Unable to write move query file." << endl;
            return false;
        }
        // Prolog expects the move in the format row,col,player.
        queryFile << row << ',' << col << ',' << currentPlayer << endl;
    }

    std::error_code ec;
    // Remove any previous response so we can tell if Prolog wrote a new one.
    std::filesystem::remove(RESPONSE_FILE, ec);

    const int exitCode = system("swipl -q -s src/prolog/rules.pl -g validate_move -t halt");
    if (exitCode != 0)
    {
        cerr << "Prolog validator exited with code " << exitCode << endl;
        return false;
    }

    ifstream responseFile(RESPONSE_FILE);
    if (!responseFile)
    {
        cerr << "Unable to read move response from Prolog." << endl;
        return false;
    }

    string response;
    responseFile >> response;
    responseFile.close();

    // Only a literal "valid" signals that the move was accepted.
    return response == "valid";
}

/*
 * getAIMoveFromScheme - request AI move from Scheme
 * parameters: none
 * returns: pair<int, int> - row and column of AI's chosen move
 */
/*
 * The Scheme AI follows a similar contract to the Prolog validator. The main
 * difference is that Scheme writes the chosen coordinates back to the response
 * file instead of the words "valid" or "invalid".
 */
pair<int, int> Game::getAIMoveFromScheme()
{
    // Share the latest board with Scheme before requesting a move.
    saveBoardState();

    std::error_code ec;
    // Remove leftovers in case the previous run crashed or was interrupted.
    std::filesystem::remove(RESPONSE_FILE, ec);

    const int exitCode = system("mit-scheme --quiet --load src/scheme/ai.scm");
    if (exitCode != 0)
    {
        cerr << "Scheme AI exited with code " << exitCode << endl;
        return make_pair(-1, -1);
    }

    ifstream responseFile(RESPONSE_FILE);
    if (!responseFile)
    {
        cerr << "Unable to read Scheme AI response." << endl;
        return make_pair(-1, -1);
    }

    string line;
    getline(responseFile, line);
    responseFile.close();

    stringstream ss(line);
    int row = -1;
    int col = -1;
    char comma = ',';
    // Parse the "row,col" string into integers; fall back to {-1,-1} if any
    // portion is missing so callers know to abort the turn.
    if (!(ss >> row))
    {
        return make_pair(-1, -1);
    }
    if (!(ss >> comma) || comma != ',')
    {
        return make_pair(-1, -1);
    }
    if (!(ss >> col))
    {
        return make_pair(-1, -1);
    }

    return make_pair(row, col);
}

/*
 * isGameOver - check if game has ended
 * parameters: none
 * returns: bool - true if game is over
 */
/*
 * Lightweight accessor used by the game loop to decide when to stop.
 */
bool Game::isGameOver() const
{
    return gameOver;
}

/*
 * getWinner - get the winning player
 * parameters: none
 * returns: int - 0 = no winner yet, 1/2 = player, -1 = draw
 */
/*
 * Returns the winner flag so the caller can print the final message.
 */
int Game::getWinner() const
{
    return winner;
}

/*
 * switchPlayer - toggle current player
 * parameters: none
 * returns: void
 */
/*
 * Flipping between player identifiers keeps alternating turns simple.
 */
void Game::switchPlayer()
{
    // Flip between 1 and 2 each turn.
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

/*
 * getCurrentPlayer - get current player number
 * parameters: none
 * returns: int - current player (1 or 2)
 */
/*
 * Used by the UI layer to decide whether to prompt a human or call the AI.
 */
int Game::getCurrentPlayer() const
{
    return currentPlayer;
}

/*
 * saveBoardState - write current board to file for other languages
 * parameters: none
 * returns: void
 */
/*
 * Persist the board in the simple CSV format shared by the other language
 * components. The first line is the size, followed by one line per row, and
 * finally the player whose turn it is.
 */
void Game::saveBoardState()
{
    ofstream stateFile(STATE_FILE);
    if (!stateFile)
    {
        cerr << "Unable to write board state file." << endl;
        return;
    }

    const int size = board.getSize();
    stateFile << size << endl;

    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            // Write the raw integer token so Prolog and Scheme can parse it.
            stateFile << board.getCell(row, col);
            if (col < size - 1)
            {
                stateFile << ',';
            }
        }
        stateFile << endl;
    }

    stateFile << currentPlayer << endl;
}

/*
 * checkWinner - determine if there's a winner
 * parameters: none
 * returns: void - updates winner and gameOver members
 */
/*
 * After every move we re-evaluate the board for wins. The checks are written so
 * they work for any square size, not just 3x3.
 */
void Game::checkWinner()
{
    winner = 0;
    gameOver = false;

    const int size = board.getSize();

    for (int row = 0; row < size; ++row)
    {
        const int value = board.getCell(row, 0);
        if (value == 0)
        {
            // An empty leading cell means this row cannot produce a winner.
            continue;
        }

        bool same = true;
        for (int col = 1; col < size; ++col)
        {
            // Break out once a mismatch is found in the row.
            if (board.getCell(row, col) != value)
            {
                same = false;
                break;
            }
        }

        if (same)
        {
            winner = value;
            gameOver = true;
            return;
        }
    }

    for (int col = 0; col < size; ++col)
    {
        const int value = board.getCell(0, col);
        if (value == 0)
        {
            // Skip columns that start empty; they cannot satisfy a win.
            continue;
        }

        bool same = true;
        for (int row = 1; row < size; ++row)
        {
            // Exit early as soon as we find a conflicting token.
            if (board.getCell(row, col) != value)
            {
                same = false;
                break;
            }
        }

        if (same)
        {
            winner = value;
            gameOver = true;
            return;
        }
    }

    int diagValue = board.getCell(0, 0);
    if (diagValue != 0)
    {
        bool same = true;
        for (int idx = 1; idx < size; ++idx)
        {
            // Compare each descending diagonal cell with the starting value.
            if (board.getCell(idx, idx) != diagValue)
            {
                same = false;
                break;
            }
        }
        if (same)
        {
            winner = diagValue;
            gameOver = true;
            return;
        }
    }

    diagValue = board.getCell(0, size - 1);
    if (diagValue != 0)
    {
        bool same = true;
        for (int idx = 1; idx < size; ++idx)
        {
            // Walk the anti-diagonal to verify it matches the owning player.
            if (board.getCell(idx, size - 1 - idx) != diagValue)
            {
                same = false;
                break;
            }
        }
        if (same)
        {
            winner = diagValue;
            gameOver = true;
            return;
        }
    }

    if (board.isFull())
    {
        // No empty cells remain and no winner was found, so declare a draw.
        winner = -1;
        gameOver = true;
    }
}

