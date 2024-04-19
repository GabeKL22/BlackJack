#ifndef SUIT_H
#define SUIT_H

#include <vector>
#include <string>

#define NUMBER_OF_SUITS 4

#define SPADES u8"\u2660"
#define HEARTS u8"\u2665"
#define DIAMONDS u8"\u2666"
#define CLUBS u8"\u2663"

using namespace std;



class Suit {
public:
    Suit() {
        suits.push_back(SPADES);
        suits.push_back(HEARTS);
        suits.push_back(DIAMONDS);
        suits.push_back(CLUBS);
    }

    vector<string> getSuits() {
        return suits;
    }

private:
    vector<string> suits;
};


#endif