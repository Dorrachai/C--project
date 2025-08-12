#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Deck.h"
#include "Dealer.h"
#include "Participant.h"
#include "FileManager.h"
#include "Button.h"

enum class Phase { None, PlayerTurn, DealerTurn, Results };

class BlackjackGame {
private:
    Deck deck;
    Dealer* dealer;
    std::vector<Participant*> players;
    FileManager fileManager;

    Phase phase = Phase::None;
    std::size_t currentPlayer = 0;

    sf::Font font;
    Button hitButton;
    Button standButton;

    std::vector<std::string> resultsLines;
    int resultsDealerValue = 0;

public:
    BlackjackGame();
    ~BlackjackGame();

    void initGame();
    void addPlayer(Participant* player);

    void startRound();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();
    void draw(sf::RenderWindow& window);
};

#endif
