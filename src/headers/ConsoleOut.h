#ifndef CONSOLE_OUT_H
#define CONSOLE_OUT_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "Game_v2.0.h"
#include "Player.h"
#include "Dealer.h"

using namespace std;

/*
    For ConsoleOut during game, v1 Game got messy with all of the printing functions and logic...
*/
class ConsoleOut {
public:

    void Dealing() {
        cout << "Dealing...\n";
    }

    void GameStart(int &numberOfPlayers) {
        cout << "Welcome to BlackJack!\n";
        cout << "How many players are playing(hands)? -> ";
        cin >> numberOfPlayers;
        cout << "\nShuffling Cards...\n";
        sleep(5);
    }

    void PlayerOptions(char &input, Player p) {
        cout << "Player " << p.GetPlayerId() << ": Hit (H) - Stand (S) - Double (D) - Split (P): ";
        cin >> input;
    }


    // This is so bad, will need to come back later to make better
    // works for now 
    // For showOutCome, we want to show when a player STANDS, or the end 
    // void Hands(Dealer dealer, vector<Player> players, bool hideDealer) {
    //     system("clear");
    //     cout << "\nDEALER\n";
    //     cout << "-------\n";
    //     cout << "\t";
    //     dealer.currentHand.DisplayHand(hideDealer);
    //     cout << "\n";
    //     string label, spacing, handstr, total, outcome;
    //     int i =1;
    //     for (auto player : players) {
    //         label += "HAND " + to_string(player.GetPlayerId()) + "             "; 
    //         spacing += "-----              ";
    //         total += "Total: " + (player.BlackJack() ? "BlackJack" : to_string(player.currentHand.GetHandValue())) + "          ";
    //         for (int i = 0; i < player.currentHand.GetHandSize(); i++) {
    //             handstr += player.currentHand.GetCards()[i] + " ";\
    //             // cout << "INDEX = " << i << endl;
    //         } 
    //         if (player.GetPlayerState() == PlayerStates_e::WIN || player.GetPlayerState() == PlayerStates_e::LOST || player.GetPlayerState() == PlayerStates_e::PUSH || player.GetPlayerState() == PlayerStates_e::STAND || player.GetPlayerState() == PlayerStates_e::BUST || player.GetPlayerState() == PlayerStates_e::DOUBLE) {
    //             outcome += player.GetPlayerStateString() + "           ";
    //         }
    //         else {
    //             outcome += "           ";
    //         }
    //         handstr += "             ";
            
    //     }
    //     label += "\n";
    //     spacing += "\n\n";
    //     handstr += "\n";
    //     total += "\n";
    //     outcome += "\n";

    //     cout << label << spacing << handstr << total << outcome;
    // }


    // Look above lol ^^^^ Thanks ChatGPT
    void Hands(Dealer dealer, vector<Player> players, bool hideDealer) {
        system("clear");
        cout << "\nDEALER\n"
                  << "-------\n"
                  << "\t";
        dealer.currentHand.DisplayHand(hideDealer);
        cout << "\n\n";

        // We will use string streams to manage each player's data
        vector<stringstream> playerLines(players.size());

        // Calculate the max width needed for the card listing
        size_t maxCardsWidth = 0;
        for (auto& player : players) {
            size_t cardsWidth = player.currentHand.GetHandSize() * 3; // Assuming each card abbreviation is two characters plus a space
            maxCardsWidth = max(maxCardsWidth, cardsWidth);
        }

        // Formatting players' hands
        for (size_t i = 0; i < players.size(); ++i) {
            auto& player = players[i];
            auto& line = playerLines[i];

            // Format and append the player hand label
            line << "HAND " << player.GetPlayerId() << ":\n";

            // Append cards with padding for alignment
            for (const auto& card : player.currentHand.GetCards()) {
                line << card << " ";
            }
            line << setw(maxCardsWidth - player.currentHand.GetHandSize() * 3) << " ";

            // Append total value or blackjack
            line << "Total: " << (player.BlackJack() ? "BlackJack" : to_string(player.currentHand.GetHandValue())) << "\n";

            // Append player's outcome state
            if (player.GetPlayerState() == PlayerStates_e::WIN || player.GetPlayerState() == PlayerStates_e::LOST || player.GetPlayerState() == PlayerStates_e::PUSH || player.GetPlayerState() == PlayerStates_e::STAND || player.GetPlayerState() == PlayerStates_e::BUST || player.GetPlayerState() == PlayerStates_e::DOUBLE) {
                line << player.GetPlayerStateString();
            }
            line << "\n\n"; // Ensure two newlines at the end of each player's section
        }

        // Print out each player's information
        for (auto& line : playerLines) {
            cout << line.str();
        }
    }


private:

};

#endif