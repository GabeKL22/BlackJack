#ifndef CURRENCY_H
#define CURRENCY_H

using namespace std;
/*
Class for the game currency, should be relatively straight-forward
We honestly wouldn't need to include this in the Dealer class because we know the dealer is always going to pay out... 
*/
class Currency {
public:
    // for this game lets just give everyone $300 to start
    Currency(bool isBank, double startingAmount) {
        amount = startingAmount;
        bank = isBank;
        bet = 0;
        lastWonAmount = 0;
    }

    void PayBlackJack(double betAmount) {
        lastWonAmount = (betAmount*2.5);
        amount += (betAmount*2.5); //3:2
    }

    void Pay(double betAmount, bool win) {
        amount += win ? (betAmount*2) : betAmount;
        lastWonAmount = win ? (betAmount*2) : betAmount;
    }

    void DoubleDown() {
        amount -= bet;
        bet = bet * 2;
    }

    bool Bet(double betAmount) {
        bet += betAmount;
        amount -= betAmount;
        if (amount < 0) {
            return false; // don't have enough 
        }
        return true;
    }

    void SetBetAmount(double betAmount) {
        bet = betAmount;
    }

    double GetBetSize() {
        return bet;
    }

    void ClearBetSize() {
        bet = 0;
    }

    double GetAmount() {
        return amount;
    }

    void SetAmount(double Amount) {
        amount = Amount;
    }

    void SetLastWonAmount(double won) {
        lastWonAmount = won;
    }

    double GetLastWonAmount() {
        return lastWonAmount;
    }

private:
    double amount;
    double bet;
    double lastWonAmount;
    bool bank; // might need this, depends (for dealer)

};

#endif