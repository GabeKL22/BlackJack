#include <iostream>

// #include "src/headers/Game.h"
#include "src/headers/Game_v2.0.h"
#include "src/headers/ConsoleOut.h"

int main() {

    // Cards c = Cards();
    // c.printDeck();
    // cout << c.getCardDeckSize();

    // v1
    // Game game = Game();
    // game.Start();


    ConsoleOut print;
    int numberOfPlayers;
    print.GameStart(numberOfPlayers);
    // v2
    Game_v2 game = Game_v2(numberOfPlayers); // starts the game
}