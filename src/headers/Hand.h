#ifndef HAND_H
#define HAND_H

#include <vector>
#include <string>
#include <iostream>
#include "Cards.h"
#include <algorithm>

using namespace std;

// simple class for the hand you currently have
class Hand {
public:
    Hand() {
        // nothing to be done just initialize it

    }
    void Add(string card) {
        cards.push_back(card);
        UpdateTotal();
    }

    void DisplayHand(bool dealerHide = false) {
        Cards c;
        cout << endl;
        if (dealerHide) {
            string value = cards[0];
            cout << value;
            cout << "\nTotal: " << c.GetCardValues(value[0]) << endl;
        }
        else {
            for (auto card : cards) {
                cout << card << " ";
            }
            cout << "\nTotal: " << total << endl;
        }
        cout << endl;

    }

    void ClearHand() {
        cards.clear();
    }

    void UpdateTotal() {
        // currently have an issue with the dealer getting A and showing the up card total at '17'
        total = 0;
        int specialCount = 0;
        Cards c;
        for (auto card : cards) {
            char cv = card[0]; // take off symbol 
            bool current_ace = false;
            if (c.CheckForAce(cv)) {
                specialCount++;
            } 
            else {
                total += c.GetCardValues(cv);
            }      
        }
        if (specialCount > 0) {
            // we have aces
            if (total <= 10 && specialCount == 1) {
                total += 11;
            }
            else if (total < 10 && specialCount > 1) {
                total += 11 + (specialCount-1);
            }
            else if (total > 11) {
                total +=1;
            }
            else {
                total += 11 + (specialCount-1);
            }
        }
    }

    int GetHandValue() {
        return total;
    }

    vector<string> GetCards() {
        return cards;
    }

    void TakeCard() {
        cards.pop_back();
    }

    void Split(string card) {
        cards.erase(remove(cards.begin(), cards.end(), card), cards.end());
    }

    int GetHandSize() {
        return cards.size();
    }

private:
    vector<string> cards; // cards in your hand
    int total; // total 
};

#endif 