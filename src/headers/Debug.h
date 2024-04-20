#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>

using namespace std;

// ANSI escape codes
string RESET = "\033[0m";
string RED = "\033[31m";
string GREEN = "\033[32m";
string YELLOW = "\033[33m";
string BLUE = "\033[34m";
string MAGENTA = "\033[35m";
string CYAN = "\033[36m";
string WHITE = "\033[37m";

enum DebugMessages_e {
    ERROR,
    PRINT,
    DEBUG
};

class Debug {
public:
    Debug() {
        
    }

    string GetMessageColor(DebugMessages_e type) {
        switch (type) {
            case ERROR:
                return RED;
            case PRINT:
                return BLUE;
            case DEBUG:
                return YELLOW;
        }
    }

    void UnknownState(DebugMessages_e type) {
        cout << GetMessageColor(type) << "UNKNOWN STATE\n";
    }

    void UnknownSetOutcome(DebugMessages_e type) {
        cout << GetMessageColor(type) << "UNHANDLED_OUTCOME -- SetOutcome()";
    }
// no need for private members
};

#endif