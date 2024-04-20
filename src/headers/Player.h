#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"

// can do action in Game while playing, get choice, then do the action of their new state
// if OK after let's say a HIT, the state would go back to PLAYING and we would run through the choices again 
enum class PlayerStates_e {
    NOT_PLAYING,
    PLAYING,
    HIT,
    BUST,
    STAND,
    SPLIT,
    DOUBLE,
    SURRENDER,
    WIN,
    LOST,
    PUSH
};



class Player {
public:
    Player(int id, int startingBalance = 0) {
        balance = startingBalance;
        playerId = id;
    }

    bool BlackJack() {
        if (currentHand.GetHandValue() == 21 && currentHand.GetHandSize() == 2) {
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

    int GetPlayerId() {
        return playerId;
    }

    string GetPlayerStateString() {
        switch (state) {
            case PlayerStates_e::WIN:
                return "Win";
            case PlayerStates_e::LOST:
                return "Lost";
            case PlayerStates_e::PUSH:
                return "Push";
            case PlayerStates_e::STAND:
                return "Stand";
            case PlayerStates_e::BUST:
                return "Busted";
            case PlayerStates_e::DOUBLE:
                return "Double";
            default:
                // Should never reach this, we would only call this after the player state is WIN, LOST
                return "Unknown State String";
        }
    }

    PlayerStates_e GetPlayerState() {
        return state;
    }

    void SetPlayerState(PlayerStates_e playerState) {
        state = playerState;
    }

    Hand currentHand;
private:
    PlayerStates_e state;
    int playerId;
    double balance;

};  

#endif