#ifndef DECK_H
#define DECK_H

#include "Cards.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <stack>

using namespace std;

static int maxCards = 0;


class Deck {
public:
    Deck(int numberOfDecks = 6) {
        // might need to set up more here late 
        // very redundant as of now 
        GenerateDecks(numberOfDecks);
    }
    
    bool isEmpty() {
        return decks.empty();
    }
    
    // rather throw them all in and shuffle then deal with an almost 2d structure (over complicates things)
    void GenerateDecks(int numberOfDecks) {
        for (int i = 0; i < numberOfDecks; i++) {
            maxCards = numberOfDecks * CARDS_IN_ONE_DECK;
            Cards c = Cards();
            vector<string> temp = c.GetCards();
            for (int j = 0; j < c.GetCardDeckSize(); j++) {
                deck.push_back(temp[j]);
            }
            // deck.push_back(c);
        }
    }

    //  
    void PrintDeck(string print, bool printAll = true, int numPrint = maxCards) {
        if (printAll)
        {
            cout << print << endl;
            int cnt = 0;
            cout << "Cards in shoe: ";
            for (auto d : deck)
            {
                cout << d << ", ";
                cnt++;
            }
            cout << "\nTotal Cards in Deck = " << cnt << endl;
        }
    }

    // shuffle the deck (entire deck)
    void ShuffleDeck(int numTimes) {
        for (int i = 0; i < numTimes; i++)
        {
            shuffle(deck.begin(), deck.end(), mt19937{std::random_device{}()});
        }
        CopyToStack();
    }

    void CopyToStack() {
        for (auto const& c : deck)
        {
            decks.push(c);
        }
    }

    void BurnCard() {
        decks.pop();
    }

    string GetCard() {
        string card = decks.top();
        decks.pop();  
        return card;
    }

private:
    vector<string> deck;
    stack<string>  decks;
};

#endif 