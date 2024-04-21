#ifndef DEALER_H
#define DEALER_H

#include <unistd.h>  // Required for sleep()
#include <string>

#include "Deck.h"
#include "Hand.h"

#define SHUFFLES 10

using namespace std;

enum class DealerStates_e {
    BLACKJACK,
    BUST,
    STAND,
    HIT,
    DEAL,
    WAIT_FOR_PLAYERS
};

class Dealer {
public:
    Dealer() : shoe(6), currentHand() {  
        shoe.ShuffleDeck(SHUFFLES);
    }

    string Deal() {
        if (!shoe.isEmpty())
        {
            return shoe.GetCard();
        }
        return "\nEmpty Shoe\n";
    }

    bool BlackJack() {
        if (currentHand.GetHandValue() == 21 && currentHand.GetHandSize() == 2) {
            return true;
        }
        else {
            return false;
        }
    }

    Hand currentHand;

    ////////////////////
    //TESTTING FUNCTIONS
    string DealSplit() {
        static bool firstHand = true;
        static string card;
        cout << card << endl;
        if (firstHand) {
             card = shoe.GetCard();
             firstHand = false;
        }
        return card;
    }

    DealerStates_e GetDealerState() {
        return state;
    }

    void SetDealerState(DealerStates_e dealerState) {
        state = dealerState;
    }

private:
    Deck shoe;
    DealerStates_e state;
};

#endif