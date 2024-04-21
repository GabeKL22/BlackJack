Weekend BlackJack Project in C++
- Console app

Running the program:
- v1.0 
- v2.0
    - Clone the repository `git clone https://github.com/GabeKL22/BlackJack.git`
    - `cd Path/To/BlackJack`
    - type `make`2, to clean the program (remove executable) type `make clean`
    - in the `Makefile` I execute a chmod command to give execute permissions to the created `BlackJack` executable. You may not need this, I did on my server. 
    - To run the BlackJack game type `./BlackJack`


Explantation of Game_v2.h:
- The Game_v2 (Game) class uses `GameState_e gameState` for it's current game state.
- `GameStateMachine()` is the main game loop, it loops forever `while(1)` or until the `gameState == END` 
- The `gameState` is initialized to STARTED in our Game_v2 Constructor. 
- The constructor takes an argument `numberOfPlayers` and run's `AddPlayer()` n times.  
- Then the GameStateMachine is started calling `StartGame()` first, this intializes our `Player` and `Dealer` states.
- Next run through the state machine the `gameState == RUNNING` so we call `RunGame()`

RunGame()
- `PlaceBets()` prompts each `Player` for a `betAmount`
- `DealGame()` deals two cards to each player, and sets the dealer state to WAIT_FOR_PLAYERS 
- Check for Dealer BlackJack, if BlackJack, check for Push
- If not we can start the game, (Prompt each player for an option)
- `SetPlayerOption(char& input, Player &p)` sets the current Players state based on console input.
- `RunPlayerOption(Player &p)` This is where the option is executed, if they hit, we give them a card, if the stand we stand, etc. 
- `CheckPlayerHand(Player &p)` Check the players hand... if they have 21 just let them go, if busted set `player.SetPlayerState(PlayerStates_e::BUST)`
- `FlipHand()` Dealer flips hand until hand value >= 17, everyone either stood, doubled, or busted at this point
- `SetOutcome()` Set the outcome of the game, check if player hands are higher than the dealer, check for push, etc.
- `GetOutcome()` Get the outcome of the game, do payouts for each player based on bet size


ConsoleOut and Debug Class:
- `class ConsoleOut` is used for ALL output in our main Game file, this cleans everything up. 
- `class Debug` is used for ERROR outputting, Debugging, etc. nice to be able to see some colors when something goes wrong, especially with console being clears every few seconds 

`Currency` Class:
- Object to hold the currency and currency actions for a player.
- Examples of Payout Functions, `PayBlackJack()`, `Pay()`.
- Examples of Player Action functions `DoubleDown()` - Increases bet size to bet size * 2. `Bet(double betAmount)`

`Player` Class:
- Pretty simple, each player has a `PlayerStates_e state` and this is our way of tracking in Game_v2.h where each player is in the game and what their current option is. 
- Each player has a `Currency balance` to keep track of their current balance and winnings. 
- `Currency& GetBalance()` returns a modifyable reference to our players current balance, which allows us to modify it via public Currency functions in our Game_v2.h file. 

`Dealer` Class:
- Dealer deals all of the cards from their private member `Deck shoe` - this is entire 6 or Deck(n) decks of cards for our BlackJack game. 
- Dealer can deal the cards by calling `dealer.Deal()` which pops a card off of our deck and returns it. 
- Also has some test functions i.e `DealSplit()` which deals a split to the players everytime. 

`Deck` Class:
- The `Deck` class uses a vector to intialize the starting deck, which is created from the `Suit` and `Cards` class (these just form a deck with card numbers (a,1-10,j,q,k)) in `GenerateDeck(int numberOfDecks)`.
- `ShuffleDeck()` shuffles `numberOfDecks` decks together, this is why we initially use a vector to shuffle it.
- After the cards are shuffled, we call `CopyToStack()` which copies our `vector<string> cards` to a `stack<string> decks`
