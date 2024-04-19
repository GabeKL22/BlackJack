#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Dealer.h"
#include <cstdlib>
#include <unistd.h>
#include <sstream>  // For stringstream
#include <iomanip>  // For setw to manage spacing
#include <termios.h>

enum GameOptions_e {
    HIT,
    STAND,
    DOUBLE,
    SPLIT
};

enum GameState_e {
    UNKNOWN,
    STARTED,
    RUNNING,
    STOPPED
};

enum PlayerState_e {
    GOOD,
    BUSTED, 
    DEALER_BLACKJACK,
    DEALER_BUSTED,
    BLACKJACK,
    WIN,
    LOST,
    PUSH,
    SPLITTING
};

class Game {
public:
    Game() {
        currentState = UNKNOWN;
    }

    void Start() {
        currentState = STARTED;
        GameState();
    }

    void PrintState() {
        // GOOD,
        // BUSTED, 
        // DEALER_BLACKJACK,
        // DEALER_BUSTED,
        // BLACKJACK,
        // WIN,
        // LOST,
        // PUSH,
        // SPLITTING
        cout << "Current State = ";
        switch (currentState) {

            case STARTED:
                cout << "STARTED_STATE\n";
                break;
            case RUNNING:
                cout << "RUNNING_STATE\n";
                break;
            case STOPPED:
                cout << "STOPPED_STATE\n";
                break;
            case UNKNOWN:
            default:
                cout << "UNKNOWN_STATE\n";
            break;
        }
    }

    void PrintPlayerState() {
        switch (playerState) {
            case SPLITTING:
                cout << "SPLIT_STATE\n";
                break;
            case PUSH:
                cout << "PUSH_STATE\n";
                break;
            case LOST:
                cout << "LOST_STATE\n";
                break;
        }
    }

    void GameState() {
        while (currentState != STOPPED)
        {   
            // PrintState();
            switch (currentState)
            {
                case STARTED:
                    StartGame();
                    break;
                case RUNNING:
                    RunGame();
                    break;
                case STOPPED:
                    Stop();
                    break;
            } 
        }
    }
    
    void StartGame() {
        currentState = RUNNING;
        Info();
        player = Player();
        dealer = Dealer();
        // initialize starting vals for game
    }

    void InitialDeal(bool specialDeal) {
        if (specialDeal) {
            player.currentHand.Add(dealer.DealSplit());
            dealer.currentHand.Add(dealer.DealSplit());
            player.currentHand.Add(dealer.DealSplit());
            dealer.currentHand.Add(dealer.DealSplit());
        }
        else {
            player.currentHand.Add(dealer.Deal());
            dealer.currentHand.Add(dealer.Deal());
            player.currentHand.Add(dealer.Deal());
            dealer.currentHand.Add(dealer.Deal());
        }
        playerState = GOOD;
    }

    void RunGame() {
        // Deal the hands, no bets yet
        static bool firstRun = true;
        InitialDeal(true);
        while (playerState == GOOD)
        // start the game
        if (player.BlackJack() && dealer.BlackJack()) {
            playerState = PUSH;
        }
        else if (dealer.BlackJack()) {
            playerState = DEALER_BLACKJACK;
        }
        else if(player.BlackJack()) {
            playerState = BLACKJACK;
        }
        else {
            // play until Player or Dealer Busts -> BUSTED
            while (playerState == GOOD) {
                char option;
                DisplayHands();
                sleep(3);
                currentOption = PrintPlayerOptions();
                firstRun = false;
                Choice(currentOption, player);
                // check for a bust
                if (playerState != SPLITTING) {
                    CheckHand(player);
                }
                // PrintPlayerState();
            }
            if (playerState == SPLITTING) {
                // need to deal a card to each one
                for (auto hand : multipleHands) {
                    // cout << "Adding cards\n";
                    hand.currentHand.Add(dealer.Deal());
                }
                for (auto hand : multipleHands) {
                    while (playerState == SPLITTING) {
                            char option;
                            DisplaySplit();
                            sleep(3);
                            currentOption = PrintPlayerOptions();
                            Choice(currentOption, hand);
                            // check for a bust
                            CheckHand(player);
                    }
                }
            }
            GetOutcome();
        }
    }

    void DisplayHands(bool dealerHide = true) {
        system("clear");
        cout << "\nDEALER\n";
        cout << "-------\n";
        cout << "\t";
        dealer.currentHand.DisplayHand(dealerHide);
        cout << "\n";

        cout << "PLAYER\n";
        cout << "-------\n";
        cout << "\t";
        player.currentHand.DisplayHand(false);
        cout << "\n";

    }

    void DisplaySplit() {
        // NOT EVEN GETTING HERE? 
        system("clear");
        cout << "\nDEALER\n";
        cout << "-------\n";
        cout << "\t";
        dealer.currentHand.DisplayHand(true);
        cout << "\n";
        string label, spacing, handstr;
        int i =1;
        bool first = true;
        for (auto hand : multipleHands) {
            if (first) {
            
            }
            label += "HAND " + to_string(i++) + "             "; 
            spacing += "-----              ";
            for (int i = 0; i < hand.currentHand.GetHandSize(); i++) {
                handstr += hand.currentHand.GetCards()[i] + " ";\
                // cout << "INDEX = " << i << endl;
            } 
            handstr += "                ";
            
        }
        label += "\n";
        spacing += "\n";
        handstr += "\n";

        cout << label << spacing << handstr;
    }
    //18 SPACES AWAY ^


    void GetOutcome() {
        DisplayHands(false);
        switch (playerState)
        {
            case WIN:
                cout << "YOU WON " << player.currentHand.GetHandValue() << " beats " << dealer.currentHand.GetHandValue() << endl;
                break;
            case BLACKJACK:
                cout << "BlackJack\n!";
                break;
            case LOST:
                cout << "YOU LOST\n";
                break;
            case PUSH:
                cout << "PUSH/TIE\n";
                break;
            case DEALER_BUSTED:
                cout << "Dealer Busted\n";
                break;
            case BUSTED:
                cout << "BUSTED " << player.currentHand.GetHandValue() << endl;
                break;
        }
        player.currentHand.ClearHand();
        dealer.currentHand.ClearHand();
        PlayAgain();
    }
  
    void PlayAgain() {
        cout << "Dealing...";
        sleep(10);
    }

    void Choice(GameOptions_e option, Player &p) {
        switch (option)
        {
            case HIT:
                p.currentHand.Add(dealer.Deal()); // could put in seperated function
                break;
            case SPLIT:
                Split(); // will need to pass an instance of Player (most likely, this can act as a "Seperate Hand")
                break;
            case DOUBLE:
            case STAND: // fall thru
            default:
                Stand();
                break;
        }
    }

    void Split() {
        // before adding split, add multiple players 
        playerState = SPLITTING;
        Player split = Player();
        split.currentHand.Add(player.currentHand.GetCards()[1]);
        // could just copy instance and take away card ? 
        player.currentHand.TakeCard();
        multipleHands.push_back(player);
        multipleHands.push_back(split);
        // PrintPlayerState();
        // Player 1 now has one card, and player 2 has the other (both size 1)

    }

    void Stand() {
        FlipHand();
        if (dealer.currentHand.GetHandValue() > 21) {
            playerState = DEALER_BUSTED;
        }
        else if(dealer.currentHand.GetHandValue() < player.currentHand.GetHandValue()) {
            playerState = WIN;
        }
        else if (dealer.currentHand.GetHandValue() > player.currentHand.GetHandValue()) {
            playerState = LOST;
        }
        else if (dealer.currentHand.GetHandValue() == player.currentHand.GetHandValue()) {
            playerState = PUSH;
        }
        
    }

    void FlipHand() {
        // will MOST LIKELY need to figure out flipping logic with Aces
        DisplayHands(false);
        sleep(1);
        while (dealer.currentHand.GetHandValue() < 17) {
            dealer.currentHand.Add(dealer.Deal());
            DisplayHands(false);
            sleep(3);
        }
    }

    GameOptions_e PrintPlayerOptions() {
        char option;
        cout << "Hit (H) - Stand (S) - Double (D) - Split (P): ";
        cin >> option;
        switch (option)
        {
            case 'D':
                return DOUBLE;
                break;
            case 'P':
                return SPLIT;
                break;
            case 'H':
                return HIT;
                break;
            case 'S':
            default:
                return STAND;
        }
    }

    bool CheckHand(Player p) {
        if (player.currentHand.GetHandValue() > 21) {
            playerState = BUSTED;
            return false;
        }
        else if (player.currentHand.GetHandValue() == 21) {
            playerState = BLACKJACK;
            return true;
        }
        return true;
    }   

    void Info() {
        // game contents
    }

    void Stop() {
        exit(1);
    }

    GameState_e GetCurrentState() {
        return currentState;
    }
private:
    Player player; // single for now 
    Dealer dealer;
    GameState_e currentState;
    GameOptions_e currentOption;
    PlayerState_e playerState;
    vector<Player> multipleHands;
    
};

#endif 