#ifndef DEALER_H
#define DEALER_H

#include <unistd.h>  // Required for sleep()
#include <string>

#include "Deck.h"
#include "Player.h"
#include "Hand.h"

#define SHUFFLES 10

using namespace std;

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

    // check for real amount later 
    void PayOut(double betAmount, Player &P) {
        P.Give(betAmount);
    }   

    void TakeMoney(double betAmount, Player &P) {
        P.Take(betAmount);
    }

    bool BlackJack() {
        if (currentHand.GetHandValue() == 21) {
            return true;
        }
        else {
            return false;
        }
    }


    // void Split() {
        // do this in game? 
    // }
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

private:
    Deck shoe;
};

#endif