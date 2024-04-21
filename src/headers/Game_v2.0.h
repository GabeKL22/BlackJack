#ifndef GAME_V2_H
#define GAME_V2_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

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
        PlaceBets();
        if (dealer.GetDealerState() == DealerStates_e::DEAL) {
            // cout << "RunGame() - Dealer State = DEAL\n";
            // deal the initial cards
            // need to dealer one to everyone in order, then again.... 
            // p1, p2, p3, d, p1, p2, p3, d
            if (DealGame()) {
                cout << "RunGame() - DealGame() Ran..\n";
                //start the game
                print.Hands(dealer, players, true);
                sleep(1); // chill for a sec
                if (dealer.BlackJack()) {
                    // no insurance atm
                    dealer.SetDealerState(DealerStates_e::BLACKJACK);
                    CheckForPush(dealer.currentHand.GetHandValue()); // check for the tie
                }
                else {
                    // need to switch to indexing, so that I don't have problems adding to it 
                    for (int it = 0; it < players.size(); it++) {
                        while (dealer.GetDealerState() == DealerStates_e::WAIT_FOR_PLAYERS && !players[it].BlackJack() && players[it].GetPlayerState() != PlayerStates_e::STAND && players[it].GetPlayerState() != PlayerStates_e::BUST) {
                            // DO ALL THE STUFF, after this dealer can do it's finishing flips 
                            char input;
                            print.PlayerOptions(input, players[it]);
                            SetPlayerOption(input, players[it]);
                            RunPlayerOption(players[it]);
                            CheckPlayerHand(players[it]);
                            print.Hands(dealer, players, true);
                            // sleep(5);
                        }
                        if (players[it].BlackJack()) {
                            // will have to write above / do the prints
                            players[it].SetPlayerState(PlayerStates_e::WIN);
                            players[it].currentHand.ClearHand();
                            players[it].GetBalance().PayBlackJack(players[it].GetBalance().GetBetSize());
                            // ConsoleLog.PrintPlayerBlackJack(players[it]))
                        }
                        // Standing/Skip this player
                    }
                    // each player played, time to flip for dealer
                    FlipHand();
                }
                SetOutcome();  // Final Outcome, everyone stood or busted
                GetOutcome();
                print.Hands(dealer, players, false, true);
                // sleep(5);
                PlayAgain();
            }

        }
    }

    bool ContainsSplit() {
        for (auto &player : players) {
            if (player.split) {
                return true;
            }
        }
        return false;
    }

    void PlayAgain() {
        print.Dealing();
        if (ContainsSplit()) {
            players = playersCopyForSplit;
        }
        for (auto &player : players) {
            player.currentHand.ClearHand();
            player.GetBalance().ClearBetSize();
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
    void PlaceBets() {
        for (auto &player : players) {
            double betAmount;
            print.AskBetAmount(player.GetPlayerId() ,betAmount);
            player.GetBalance().Bet(betAmount);
        }
    }

    void GetOutcome() {
        for (auto &player : players) {
            switch (player.GetPlayerState()) {
                case PlayerStates_e::WIN:
                    player.GetBalance().Pay(player.GetBalance().GetBetSize(), true);
                    break;
                case PlayerStates_e::LOST:
                    // don't have to do anything rn, technically took it away already
                    player.GetBalance().SetLastWonAmount(0);
                    break;
                case PlayerStates_e::PUSH:
                    // do nothing
                    player.GetBalance().Pay(player.GetBalance().GetBetSize(), false);
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
                // TODO: Hit(p)?
                p.currentHand.Add(dealer.Deal());
                break;
            }
            case PlayerStates_e::DOUBLE:
            {
                p.currentHand.Add(dealer.Deal());
                p.SetPlayerState(PlayerStates_e::STAND);
                p.GetBalance().DoubleDown();
                break;
            }
            case PlayerStates_e::SPLIT:
            {
                // Do Splitting stuff
                Split(p);
                break;
            }
            case PlayerStates_e::STAND:
            default:   // fall through
            {
                break;
            }

        }
    };

    /*
    We're splitting, need to create an Instance of Player and take the second card
    from the calling instance and give it to the new one

    Splits:
        10, 10
        split, Original hand hits first, keeps hitting (unless ACE), until stand or bust 
    */
    void Split(Player &p) {
        playersCopyForSplit = players;
        // fix player stuff, NEED TO FIGURE OUT BALANCE

        Player temp = Player(p.GetPlayerId(), true); 
        p.GetBalance().Bet(p.GetBalance().GetBetSize());
        temp.GetBalance().SetAmount(p.GetBalance().GetAmount());
        temp.currentHand.Add(p.currentHand.GetCards()[1]); // add their second card
        temp.GetBalance().SetBetAmount(p.GetBalance().GetBetSize());
        temp.split = true;
        p.hasSplit = true;
        p.currentHand.TakeCard(); // take it away from first hand 

        // deal them two cards
        p.currentHand.Add(dealer.Deal());
        temp.currentHand.Add(dealer.Deal());

        // add split to Players vector
        players.push_back(temp);
        SortPlayerIdsForSplit();
        print.Hands(dealer, players, true);
    }

    void SortPlayerIdsForSplit() {
        // Sort the vector by player ID
        // pretty cool stuff here
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.GetPlayerId() < b.GetPlayerId();
        });
    }

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

    /*
    Return the index of the player vector that matches the giving ID
    might not even need this lol
    */
    int GetPlayerById(int findId) {
        for (int i = 0; i < players.size(); i++) {
            if(players[i].GetPlayerId() == findId) {
                return i;
            }
        }
        return -1; // could not find ID
    }
private:
    ConsoleOut print;
    Debug debug;
    vector<Player> players;
    vector<Player> playersCopyForSplit; // might have a problem after PlayAgain() without copying to this 
    Dealer dealer;
    GameStates_e gameState;
};

#endif