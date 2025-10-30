/*
 * main.cpp
 * Entry point for the cross-language board game system
 */

#include "game.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Cross-Language Board Game System ===" << endl;
    cout << "Starting with Tic-Tac-Toe (3x3)" << endl;
    cout << endl;

    // Temporary manual Board test
    Board b(3);
    b.display();
    b.setCell(0, 0, 1);
    b.setCell(1, 1, 2);
    cout << endl << "After two moves:" << endl;
    b.display();
    
    // TODO: main game loop
    // while (!game.isGameOver()) {
    //     game.display();
    //     
    //     if (current player is human) {
    //         get move from user input
    //         validate move via prolog
    //         if valid, apply move
    //     } else {
    //         request AI move from scheme
    //         apply move
    //     }
    //     
    //     check for winner
    //     switch players
    // }
    
    // TODO: display final result
    // game.display();
    // announce winner or draw
    
    cout << "(Temporary board test complete)" << endl;
    
    return 0;
}

