#ifndef CARDS_H
#define CARDS_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

#include "Suit.h"

using namespace std;

#define SUITS 4
#define CARDS_IN_SUIT 13
#define CARDS_IN_ONE_DECK SUITS * CARDS_IN_SUIT

/*
Just need to generate the cards into a vector. 

Generate a deck of cards, then use these cards with a parameter in Deck.h to create the Deck for the game

Here -> Creating Deck
Deck.h -> generateDeck(*Cards, int numberOfDecks) 
 = generate n (numberOfDecks) many Cards

 Map Cards like:
    A (H) : we can tag this with something specific to signify it being 1 or 11 
    2 (S) : 2
    ...
    ..
    .
    K (S) : 10
*/
class Cards {
public:

    Cards() {
        GenerateCards();
    }

    bool CheckForAce(char value) {
        bool retVal = false;
        if (value == 'A') {
            retVal = true;
        }
        return retVal;
    }
    // can just call this outside of the function, most likely in Game.h
    // when passing to function - pass the string[0] (dont wnt to read the suit, leave that up to game.h for pairs, etc)
    int GetCardValues(char value) {
        switch (value)
        {
            case 'A':
                return 11;
            case 'T':
            case 'J':
            case 'Q':
            case 'K':
                return 10;
            default:
                return value - '0';

        }
    }

    void GenerateCards() {
        Suit s = Suit();
        for (string suit : s.getSuits())
        {
            for (int i = 0; i < CARDS_IN_SUIT; i++)
            {
                cards.push_back(values[i] + suit);
            }
        }
    }

    void PrintDeck() {
        for (const auto& c : cards) {
            cout << c << ", ";
        }
        cout << "Size = " << GetCardDeckSize();
    }

    // come back to this later
    int GetCardValue(string c) {
        // fill in 
        return 0;
    }

    int GetCardDeckSize() {
        return cards.size();
    }

    vector<string> GetCards() {
        return cards;
    }
private:
    char values[CARDS_IN_SUIT] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
    // map<string, int> cards; // suit -> value
    vector<string> cards;

    

};

#endif