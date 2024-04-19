#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"

class Player {
public:
    Player(int startingBalance = 0) {
        balance = startingBalance;
    }

    bool BlackJack() {
        if (currentHand.GetHandValue() == 21) {
            return true;
        }
        else {
            return false;
        }
    }
    

    // keep track of winning in Game.h
    void Give(double amount) {
        balance += amount;
    }

    // can take out for now, remeber to 
    void Take(double amount) {
        balance -= amount;
    }

    double Balance() {
        return balance;
    }

    // figure out in game, pay out double (bet + winning) to balance
    void PlaceBet(double amount) {
        Take(amount);
    }



    Hand currentHand;
private:
    double balance;

};  

#endif