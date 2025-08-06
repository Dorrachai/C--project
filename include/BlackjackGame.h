#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <vector>
#include "Deck.h"
#include "Dealer.h"
#include "Participant.h"
#include "FileManager.h"

class BlackjackGame {
private:
    Deck deck;
    Dealer* dealer;
    std::vector<Participant*> players;
    FileManager fileManager;

public:
    BlackjackGame();
    ~BlackjackGame();
    
    void initGame();
    void playRound();
    void addPlayer(Participant* player);
};

#endif
