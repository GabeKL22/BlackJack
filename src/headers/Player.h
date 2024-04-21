#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include "Currency.h"

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
    Player(int id, double startingBalance = 300, bool isSplit = false)
        : playerId(id), balance(false, startingBalance), split(isSplit) {
    }

    bool BlackJack() {
        if (currentHand.GetHandValue() == 21 && currentHand.GetHandSize() == 2) {
            return true;
        }
        else {
            return false;
        }
    }
    
    int GetPlayerId() const {
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

    Currency& GetBalance() {
        return balance;
    }

    // simulate a split as another Player
    // Keeping track for UI Output
    bool split; // for split
    bool hasSplit; // for original

    Hand currentHand;
private:
    PlayerStates_e state;
    int playerId;
    Currency balance;



};  

#endif