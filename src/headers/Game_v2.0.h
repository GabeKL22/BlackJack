#ifndef GAME_V2_H
#define GAME_V2_H

#include <vector>
#include <string>
#include <iostream>

#include "Player.h"
#include "Dealer.h"
#include "ConsoleOut.h"
#include "Debug.h"


/*
    STARTED: Setting everything up to play
    RUNNING: Game is currently running (hands are being dealt and everything is set up)
    STOPPED: No one is playing...
*/
enum GameStates_e {
    STARTED,
    RUNNING,
    STOPPED,
    END
};



/*
 Main Game Logic
*/
class Game_v2 {
public:
    /*
    Constructor
    */
    Game_v2(int numberOfPlayers) {
        gameState = STARTED;
        // start output
        debug = Debug();
        print = ConsoleOut();
        for (int i = 0; i < numberOfPlayers; i++) {
            AddPlayer();
        }
        // Player vector is initialized
        while (1) {
            GameStateMachine();
        }
    }

    // overall StateMachine Loop, loops forever until gameState = END
    void GameStateMachine() {
        switch (gameState) {
            case STARTED:
                // cout << "GameStateMachine() - STARTED\n";
                StartGame();
                break;
            case RUNNING:
                // cout << "GameStateMachine() - RUNNING\n";
                RunGame();
                break;
            case STOPPED:
                break;
            case END:
            default:
                if (gameState != END) {
                    debug.UnknownState(ERROR);
                }
                exit(1);
                break;
        }
    }

    void RunGame() {
        if (dealer.GetDealerState() == DealerStates_e::DEAL) {
            // cout << "RunGame() - Dealer State = DEAL\n";
            // deal the initial cards
            // need to dealer one to everyone in order, then again.... 
            // p1, p2, p3, d, p1, p2, p3, d
            if (DealGame()) {
                cout << "RunGame() - DealGame() Ran..\n";
                //start the game
                print.Hands(dealer, players, true);
                sleep(5); // chill for a sec
                if (dealer.BlackJack()) {
                    // no insurance atm
                    dealer.SetDealerState(DealerStates_e::BLACKJACK);
                    CheckForPush(dealer.currentHand.GetHandValue()); // check for the tie
                }
                else {
                    for (auto &player : players) {
                        while (dealer.GetDealerState() == DealerStates_e::WAIT_FOR_PLAYERS && !player.BlackJack() && player.GetPlayerState() != PlayerStates_e::STAND && player.GetPlayerState() != PlayerStates_e::BUST) {
                            // DO ALL THE STUFF, after this dealer can do it's finishing flips 
                            char input;
                            print.PlayerOptions(input, player);
                            SetPlayerOption(input, player);
                            RunPlayerOption(player);
                            CheckPlayerHand(player);
                            print.Hands(dealer, players, true);
                            sleep(5);
                        }
                        if (player.BlackJack()) {
                            // will have to write above / do the prints
                            player.SetPlayerState(PlayerStates_e::WIN);
                            player.currentHand.ClearHand();
                            // PayoutBlackJack(player.GetBetSize());
                            // ConsoleLog.PrintPlayerBlackJack(player))
                        }
                        // Standing/Skip this player
                    }
                    // each player played, time to flip for dealer
                    FlipHand();
                }
                SetOutcome();  // Final Outcome, everyone stood or busted
                GetOutcome();
                print.Hands(dealer, players, false);
                sleep(5);
                PlayAgain();
            }

        }
    }

    void PlayAgain() {
        print.Dealing();
        for (auto &player : players) {
            player.currentHand.ClearHand();
            player.SetPlayerState(PlayerStates_e::PLAYING);
        }
        dealer.currentHand.ClearHand();
        dealer.SetDealerState(DealerStates_e::DEAL);
        sleep(5);
    }

    // set the states, initialize the rest before running game
    void StartGame() {
        gameState = RUNNING;
        dealer = Dealer();
        for (auto &player : players) {
            // set all player states to playing..
            player.SetPlayerState(PlayerStates_e::PLAYING);
        }
        dealer.SetDealerState(DealerStates_e::DEAL);
    }

    // Special instructions
    /*
    Pay the players
    */
    void GetOutcome() {
        for (auto &player : players) {
            switch (player.GetPlayerState()) {
                case PlayerStates_e::WIN:
                    // player.PayOut(player.BetSize());
                    break;
                case PlayerStates_e::LOST:
                    // player.Take(player.BetSize());
                    break;
                case PlayerStates_e::PUSH:
                    // do nothing
                    break;
            }
        }
    }

    /*
    Set the outcome of the match

    This involves setting the player states to WIN, PUSH, or LOST
    */
   void SetOutcome() {
        //check every player vs the dealer
        switch (dealer.GetDealerState()) {
            case DealerStates_e::BUST:
            {
                // everyone that didn't BUST wins!
                for (auto &player : players) {
                    if (player.GetPlayerState() != PlayerStates_e::BUST) {
                        player.SetPlayerState(PlayerStates_e::WIN);
                    }   
                    else { //busted already, you lose
                        player.SetPlayerState(PlayerStates_e::LOST);
                    }
                }
                break;
            }
            case DealerStates_e::STAND:
            {
                // check each player to see if our value is higher
                for (auto &player : players) {
                    if (player.GetPlayerState() != PlayerStates_e::BUST) {
                        // win, push, tie
                        if (player.currentHand.GetHandValue() > dealer.currentHand.GetHandValue()) {
                            //win
                            player.SetPlayerState(PlayerStates_e::WIN);
                        }
                        else if (player.currentHand.GetHandValue() < dealer.currentHand.GetHandValue()) {
                            // lose
                            player.SetPlayerState(PlayerStates_e::LOST);
                        }
                        else {
                            //push
                            player.SetPlayerState(PlayerStates_e::PUSH);
                        }
                    }
                }
                break;
            }
            default:
                debug.UnknownSetOutcome(ERROR);
                break;
        }
        // if (dealer.GetDealerState() == DealerStates_e::BUST) {

        // }
        // else if (dealer.GetDealerState() == DealerStates_e::STAND) {

        // }
   }

    /*
    Check Player Hand for a bust
    */
    void CheckPlayerHand(Player &p) {
        if (p.currentHand.GetHandValue() > 21) {
            p.SetPlayerState(PlayerStates_e::BUST);
        }
        else if (p.currentHand.GetHandValue() == 21) {
            // just stand on a 21 lol
            p.SetPlayerState(PlayerStates_e::STAND);
        }
    }

    /*
    Everyone played, time for dealer to reveal and flip to 17
    */
    void FlipHand() {
        // will MOST LIKELY need to figure out flipping logic with Aces
        sleep(1);
        while (dealer.currentHand.GetHandValue() < 17) {
            dealer.currentHand.Add(dealer.Deal());
            print.Hands(dealer, players, false);
            sleep(3);
        }
        CheckDealerHand();
    }


    void CheckDealerHand() {
        if (dealer.currentHand.GetHandValue() > 21) {
            dealer.SetDealerState(DealerStates_e::BUST);
        }
        else if (dealer.currentHand.GetHandValue() >= 17) {
            dealer.SetDealerState(DealerStates_e::STAND);
        }
    }

    /*
    Set the players option
    */
    void SetPlayerOption(char selection, Player &p) {
        switch (selection) {
            case 'H':
                p.SetPlayerState(PlayerStates_e::HIT);
                break;
            case 'D':
                p.SetPlayerState(PlayerStates_e::DOUBLE);
                break;
            case 'P':
                p.SetPlayerState(PlayerStates_e::SPLIT);
                break;
            case 'X':
                p.SetPlayerState(PlayerStates_e::SURRENDER);
                break;
            case 'S':
            default:
                p.SetPlayerState(PlayerStates_e::STAND);
                // do nothing
        }
    }


    /*
    Run the players option
    */
    void RunPlayerOption(Player &p) {
        switch (p.GetPlayerState()) {
            case PlayerStates_e::HIT:
            {
                p.currentHand.Add(dealer.Deal());
                break;
            }
            case PlayerStates_e::DOUBLE:
            {
                // Do balance stuff here

                //
                p.currentHand.Add(dealer.Deal());
                p.SetPlayerState(PlayerStates_e::STAND);
                break;
            }
            case PlayerStates_e::SPLIT:
            {
                // Do Splitting stuff
                // Split(p);
            }
            case PlayerStates_e::STAND:
            default:   // fall through
            {
                break;
            }

        }
    };

    /*
    Stand/Do Nothing
    */
   void Hit(Player &p) {
        p.currentHand.Add(dealer.Deal());
   }

    /*
    Check for a push/tie with the dealer
    */
    void CheckForPush(int value) {
        // loop through each and set state to push or lost
        for (auto &player : players) {
            if (player.currentHand.GetHandValue() == dealer.currentHand.GetHandValue()) {
                player.SetPlayerState(PlayerStates_e::PUSH);
            }
        }
    }

    /*
    Do the intial Deal for the game, each player gets 2 cards
    */
    bool DealGame() {
        int rounds = 1;
        bool retVal = false;
        while (rounds <= 2 && rounds++) {
            for (auto &player : players) {
                player.currentHand.Add(dealer.Deal());
                if (player.currentHand.GetHandSize() == 2) {
                    retVal = true;
                }
            }
            dealer.currentHand.Add(dealer.Deal());
        }
        dealer.SetDealerState(DealerStates_e::WAIT_FOR_PLAYERS);    
        return retVal && dealer.currentHand.GetHandSize() == 2;

        // each player, and the dealer have 2 cards
    }

    void AddPlayer() {
        static int playerId = 1;
        Player p = Player(playerId++);
        players.push_back(p);
    }

private:
    ConsoleOut print;
    Debug debug;
    vector<Player> players;
    Dealer dealer;
    GameStates_e gameState;
};

#endif