#include <SFML/Graphics.hpp>
#include "../include/BlackjackGame.h"
#include "../include/Player.h"
#include "../include/Button.h"
#include <iostream>
#include <sstream>

static void drawCard(sf::RenderWindow& window, const Card& card, float x, float y, sf::Font& font) {
    sf::RectangleShape cardShape(sf::Vector2f(60.f, 90.f));
    cardShape.setFillColor(sf::Color::White);
    cardShape.setOutlineColor(sf::Color::Black);
    cardShape.setOutlineThickness(2);
    cardShape.setPosition(x, y);

    sf::Text cardText;
    cardText.setFont(font);
    cardText.setString(card.toString());
    cardText.setCharacterSize(24);
    cardText.setFillColor(sf::Color::Black);
    cardText.setPosition(x + 10, y + 30);

    window.draw(cardShape);
    window.draw(cardText);
}

BlackjackGame::BlackjackGame()
    : hitButton("Hit",   200.f, 600.f, font)
    , standButton("Stand", 400.f, 600.f, font) {

    dealer = new Dealer();

    if (!font.loadFromFile("font/comic.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    hitButton.setFont(font);
    standButton.setFont(font);

    phase = Phase::None;
}

BlackjackGame::~BlackjackGame() {
    delete dealer;
    for (Participant* p : players) delete p;
}

void BlackjackGame::initGame() {
    std::cout << "Blackjack Game Initialized" << std::endl;
    deck.shuffle();
}

void BlackjackGame::addPlayer(Participant* player) {
    if (!player) return;
    players.push_back(player);
    std::cout << "[ADD] Player added: " << player->getName()
            << "   total=" << players.size() << "\n";
}

void BlackjackGame::startRound() {

        if (players.empty()) {
        std::cerr << "[GUARD] startRound() aborted: no players added\n";
        phase = Phase::None;
        return;
    }

    std::cout << "\n=== NEW ROUND ===" << std::endl;

    for (auto* p : players) p->clearHand();
    dealer->clearHand();
    resultsLines.clear();
    resultsDealerValue = 0;

    int minNeeded = static_cast<int>(players.size()) * 2 + 2 + 20;
    if (deck.remaining() < static_cast<std::size_t>(minNeeded)) {
        deck.reset();
    }

    for (int i = 0; i < 2; ++i) {
        for (auto* p : players) {
            if (deck.remaining() == 0) deck.reset();
            Card c = deck.dealCard();
            p->addCard(c);
        }
        Card d = deck.dealCard();
        dealer->addCard(d);
    }

    std::cout << "Dealer: " << dealer->getHand()->toString() << "\n";
    for (auto* p : players) {
        std::cout << p->getName() << ": " << p->getHand()->toString() << "\n";
    }

    currentPlayer = 0;
    phase = players.empty() ? Phase::DealerTurn : Phase::PlayerTurn;
}

void BlackjackGame::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (phase != Phase::PlayerTurn || players.empty()) return;

    auto* p = players[currentPlayer];

    if (hitButton.isClicked(event, window)) {
        if (deck.remaining() == 0) deck.reset();  
        Card c = deck.dealCard();
        p->addCard(c);
        std::cout << p->getName() << " hits: " << c.toString()
                << " -> " << p->getHand()->getValue() << std::endl;

        if (p->getHandValue() > 21) {
            std::cout << p->getName() << " busts!" << std::endl;
            currentPlayer++;
            if (currentPlayer >= players.size()) {
                phase = Phase::DealerTurn;
            }
        }
    }

    if (standButton.isClicked(event, window)) {
        std::cout << p->getName() << " stands with " << p->getHandValue() << std::endl;
        currentPlayer++;
        if (currentPlayer >= players.size()) {
            phase = Phase::DealerTurn;
        }
    }
}

void BlackjackGame::update() {
    if (phase == Phase::DealerTurn) {
        dealer->play();
        while (dealer->getHandValue() < 17) {
            if (deck.remaining() == 0) deck.reset();  
            Card c = deck.dealCard();
            dealer->addCard(c);
            std::cout << "Dealer draws: " << c.toString() << std::endl;
        }

        resultsLines.clear();
        resultsDealerValue = dealer->getHandValue();

        std::cout << "\n=== RESULTS ===" << std::endl;
        std::ostringstream head;
        head << "Dealer: " << dealer->getHand()->toString();
        resultsLines.push_back(head.str());

        for (auto* p : players) {
            int pv = p->getHandValue();
            int dv = resultsDealerValue;

            std::string verdict;
            if (pv > 21) {
                verdict = "LOSE (bust)";
            } else if (dv > 21) {
                verdict = "WIN (dealer bust)";
            } else if (pv > dv) {
                verdict = "WIN";
            } else if (pv < dv) {
                verdict = "LOSE";
            } else {
                verdict = "TIE";
            }

            std::ostringstream row;
            row << p->getName() << ": " << p->getHand()->toString() << "  →  " << verdict;
            resultsLines.push_back(row.str());

            std::cout << row.str() << std::endl;
        }

        phase = Phase::Results;
    }
}

void BlackjackGame::draw(sf::RenderWindow& window) {
    float dealerX = 400.f;
    float dealerY = 50.f;
    const auto& dealerCards = dealer->getHand()->getCards();
    for (size_t i = 0; i < dealerCards.size(); ++i) {
        drawCard(window, dealerCards[i], dealerX + i * 72.f, dealerY, font);
    }

    float playerYStart = 250.f;
    float rowSpacing   = 170.f;
    float cardSpacing  = 72.f;

    int idx = 0;
    for (auto* p : players) {
        float nameX = 60.f;
        float nameY = playerYStart + idx * rowSpacing - 30.f;

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(p->getName());
        nameText.setCharacterSize(24);
        if (phase == Phase::PlayerTurn && static_cast<std::size_t>(idx) == currentPlayer) {
            nameText.setFillColor(sf::Color(255, 215, 0));
        } else {
            nameText.setFillColor(sf::Color::White);
        }
        nameText.setPosition(nameX, nameY);
        window.draw(nameText);

        float cardX = 60.f;
        float cardY = playerYStart + idx * rowSpacing;

        const auto& cards = p->getHand()->getCards();
        for (size_t j = 0; j < cards.size(); ++j) {
            drawCard(window, cards[j], cardX + j * cardSpacing, cardY, font);
        }
        ++idx;
    }

    if (phase == Phase::PlayerTurn) {
        hitButton.draw(window);
        standButton.draw(window);

        if (!players.empty()) {
            sf::Text turnText("Turn: " + players[currentPlayer]->getName(), font, 22);
            turnText.setFillColor(sf::Color::White);
            turnText.setPosition(50.f, 560.f);
            window.draw(turnText);
        }
    }

    if (phase == Phase::Results) {
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(overlay);

        float panelW = 720.f, panelH = 320.f;
        sf::RectangleShape panel(sf::Vector2f(panelW, panelH));
        panel.setPosition((window.getSize().x - panelW) / 2.f, (window.getSize().y - panelH) / 2.f);
        panel.setFillColor(sf::Color(20, 20, 20, 230));
        panel.setOutlineColor(sf::Color(255, 255, 255, 180));
        panel.setOutlineThickness(2.f);
        window.draw(panel);

        float px = panel.getPosition().x + 24.f;
        float py = panel.getPosition().y + 20.f;

        sf::Text title("Results", font, 28);
        title.setFillColor(sf::Color::White);
        title.setPosition(px, py);
        window.draw(title);

        float lineY = py + 50.f;
        float lineStep = 34.f;

        for (std::size_t i = 0; i < resultsLines.size(); ++i) {
            sf::Text line(resultsLines[i], font, 22);
            if (i == 0) {
                line.setFillColor(sf::Color(173, 216, 230));
            } else if (resultsLines[i].find("WIN") != std::string::npos) {
                line.setFillColor(sf::Color(0, 255, 0));
            } else if (resultsLines[i].find("LOSE") != std::string::npos) {
                line.setFillColor(sf::Color(255, 100, 100));
            } else {
                line.setFillColor(sf::Color(255, 215, 0));
            }
            line.setPosition(px, lineY + i * lineStep);
            window.draw(line);
        }

        sf::Text hint("Press Enter for new round", font, 20);
        hint.setFillColor(sf::Color(200, 200, 200));
        hint.setPosition(px, panel.getPosition().y + panelH - 36.f);
        window.draw(hint);
    }
}
