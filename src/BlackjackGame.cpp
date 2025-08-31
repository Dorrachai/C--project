#include <SFML/Graphics.hpp>
#include "../include/BlackjackGame.h"
#include "../include/Player.h"
#include "../include/Button.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <exception> 

static const char* defaultVerdict(int pv, int dv) {
    if (pv > 21) return "LOSE (bust)";
    if (dv > 21) return "WIN (dealer bust)";
    if (pv > dv)  return "WIN";
    if (pv < dv)  return "LOSE";
    return "TIE";
}

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

    dealer = std::unique_ptr<Dealer>(new Dealer());
    verdictFn = &defaultVerdict;

    if (!font.loadFromFile("font/comic.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    hitButton.setFont(font);
    standButton.setFont(font);

    phase = Phase::None;

    totalWins = fileManager.loadTotalWins();
}

BlackjackGame::~BlackjackGame() = default;

void BlackjackGame::initGame() {
    std::cout << "Blackjack Game Initialized" << std::endl;
    deck.shuffle();
}

void BlackjackGame::addPlayer(std::unique_ptr<Participant> player) {
    players.emplace_back(std::move(player));
}

void BlackjackGame::startRound() {
    std::cout << "\n=== NEW ROUND ===" << std::endl;

    for (auto& up : players) up->clearHand();
    dealer->clearHand();
    resultsLines.clear();
    resultsDealerValue = 0;

    int minNeeded = static_cast<int>(players.size()) * 2 + 2 + 20;
    if (deck.remaining() < static_cast<std::size_t>(minNeeded)) {
        deck.reset();
    }

    try {
        for (int i = 0; i < 2; ++i) {
            for (auto& up : players) {
                Card c = deck.dealCard();
                up->addCard(c);
            }
            Card d = deck.dealCard();
            dealer->addCard(d);
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error starting round: " << ex.what() << std::endl;
        resultsLines.clear();
        resultsLines.push_back(std::string("ERROR: ") + ex.what());
        phase = Phase::Results;
        return;
    }

    std::cout << "Dealer: " << dealer->getHand()->toString() << "\n";
    for (auto& up : players) {
        std::cout << up->getName() << ": " << up->getHand()->toString() << "\n";
    }

    currentPlayer = 0;
    phase = players.empty() ? Phase::DealerTurn : Phase::PlayerTurn;
}

void BlackjackGame::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (phase != Phase::PlayerTurn || players.empty()) return;
    if (currentPlayer >= players.size()) return;

    Participant* p = players[currentPlayer].get();

    if (hitButton.isClicked(event, window)) {
        try {
            Card c = deck.dealCard();
            p->addCard(c);
            std::cout << p->getName() << " hits: " << c.toString()
                    << " -> " << p->getHand()->getValue() << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error on hit: " << ex.what() << std::endl;
            resultsLines.clear();
            resultsLines.push_back(std::string("ERROR: ") + ex.what());
            phase = Phase::Results;
            return;
        }

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
    try {
        while (dealer->getHandValue() < 17) {
            Card c = deck.dealCard();
            dealer->addCard(c);
            std::cout << "Dealer draws: " << c.toString() << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error dealing to dealer: " << ex.what() << std::endl;
        resultsLines.clear();
        resultsLines.push_back(std::string("ERROR: ") + ex.what());
        phase = Phase::Results;
        return;
    }

        resultsLines.clear();
        resultsDealerValue = dealer->getHandValue();

        std::cout << "\n=== RESULTS ===" << std::endl;
        std::ostringstream head;
        head << "Dealer: " << dealer->getHand()->toString();
        resultsLines.push_back(head.str());

        int roundWins = 0;

        for (auto& up : players) {
            int pv = up->getHandValue();
            int dv = resultsDealerValue;

            const char* verdict = verdictFn(pv, dv);

            if (std::string(verdict).find("WIN") != std::string::npos) {
                roundWins += 1;
            }

            std::ostringstream row;
            row << up->getName() << ": " << up->getHand()->toString()
                << "  →  " << verdict;
            resultsLines.push_back(row.str());

            std::cout << row.str() << std::endl;
        }

        if (roundWins > 0) {
            totalWins += roundWins;
            fileManager.saveTotalWins(totalWins);
            std::cout << "Total wins (all players combined): " << totalWins << std::endl;
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
    for (auto& up : players) {
        float nameX = 60.f;
        float nameY = playerYStart + idx * rowSpacing - 30.f;

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(up->getName());
        nameText.setCharacterSize(24);
        if (phase == Phase::PlayerTurn && static_cast<std::size_t>(idx) == currentPlayer) {
            nameText.setFillColor(sf::Color(255, 215, 0));
        } else {
            nameText.setFillColor(sf::Color::Blue);
        }
        nameText.setPosition(nameX, nameY);
        window.draw(nameText);

        float cardX = 60.f;
        float cardY = playerYStart + idx * rowSpacing;

        const auto& cards = up->getHand()->getCards();
        for (size_t j = 0; j < cards.size(); ++j) {
            drawCard(window, cards[j], cardX + j * cardSpacing, cardY, font);
        }
        ++idx;
    }

    if (phase == Phase::PlayerTurn) {
        hitButton.draw(window);
        standButton.draw(window);

        if (!players.empty() && currentPlayer < players.size()) {
            sf::Text turnText("Turn: " + players[currentPlayer]->getName(), font, 22);
            turnText.setFillColor(sf::Color::Blue);
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

        static const auto colorFor = [](const std::string& s) {
            if (s.rfind("Dealer:", 0) == 0) return sf::Color(173, 216, 230);
            if (s.find("WIN")  != std::string::npos) return sf::Color(0, 255, 0);
            if (s.find("LOSE") != std::string::npos) return sf::Color(255, 100, 100);
            return sf::Color(255, 215, 0);
        };

        for (std::size_t i = 0; i < resultsLines.size(); ++i) {
            sf::Text line(resultsLines[i], font, 22);
            line.setFillColor(colorFor(resultsLines[i]));
            line.setPosition(px, lineY + i * lineStep);
            window.draw(line);
        }

        sf::Text totalWinsText("Total wins: " + std::to_string(totalWins), font, 22);
        totalWinsText.setFillColor(sf::Color::Cyan);
        totalWinsText.setPosition(px, py + panelH - 70.f);
        window.draw(totalWinsText);

        sf::Text hint("Press Enter for new round", font, 20);
        hint.setFillColor(sf::Color(200, 200, 200));
        hint.setPosition(px, panel.getPosition().y + panelH - 36.f);
        window.draw(hint);
    }
}

void BlackjackGame::run() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Blackjack", sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Texture homeTexture, gameTexture;
    sf::Sprite  homeSprite,  gameSprite;
    bool hasHomeBackground = false, hasGameBackground = false;

    auto scaleBackgrounds = [&](){
        if (hasHomeBackground) {
            sf::Vector2u ts = homeTexture.getSize(), ws = window.getSize();
            homeSprite.setScale(float(ws.x)/ts.x, float(ws.y)/ts.y);
        }
        if (hasGameBackground) {
            sf::Vector2u ts = gameTexture.getSize(), ws = window.getSize();
            gameSprite.setScale(float(ws.x)/ts.x, float(ws.y)/ts.y);
        }
    };

    if (homeTexture.loadFromFile("home.png")) {
        homeSprite.setTexture(homeTexture);
        hasHomeBackground = true;
        scaleBackgrounds();
    }
    if (gameTexture.loadFromFile("bj_table.jpg")) {
        gameSprite.setTexture(gameTexture);
        hasGameBackground = true;
        scaleBackgrounds();
    }
    sf::Text inputText("Enter number of players: ", font, 24);
    inputText.setPosition(50.f, 50.f);

    sf::Text playerNamesText("Enter player names:", font, 24);
    playerNamesText.setPosition(50.f, 100.f);

    int numPlayers = 0;
    std::vector<std::string> playerNames;
    std::string currentInput;
    bool inputNumberOfPlayers = true;
    bool inputCompleted = false;
    bool gameStarted = false;
    int  currentPlayerIndex = 0;

    initGame();


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (!inputCompleted) {
                if (event.type == sf::Event::TextEntered) {
                    if (inputNumberOfPlayers) {
                        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                            currentInput += static_cast<char>(event.text.unicode);
                        } else if (event.text.unicode == 8 && !currentInput.empty()) {
                            currentInput.pop_back();
                        } else if (event.text.unicode == 13 && !currentInput.empty()) {
                            numPlayers = std::stoi(currentInput);
                            currentInput.clear();
                            inputNumberOfPlayers = false;
                            std::cout << "Number of players: " << numPlayers << std::endl;
                        }
                    } else {
                        if (event.text.unicode == 8 && !currentInput.empty()) {
                            currentInput.pop_back();
                        } else if (event.text.unicode == 13 && !currentInput.empty()) {
                            playerNames.push_back(currentInput);
                            currentInput.clear();
                            currentPlayerIndex++;
                            if (currentPlayerIndex >= numPlayers) {
                                inputCompleted = true;
                            }
                        } else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                            currentInput += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            } else {
                handleEvent(event, window);

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    startRound();
                }
            }
        }

        if (!inputCompleted && hasHomeBackground) {
            window.clear();
            window.draw(homeSprite);
        } else if (inputCompleted && hasGameBackground) {
            window.clear();
            window.draw(gameSprite);
        } else {
            window.clear(sf::Color::Green);
        }

        if (!inputCompleted) {
            window.draw(inputText);
            if (inputNumberOfPlayers) {
                sf::Text numberInputText(currentInput, font, 24);
                numberInputText.setPosition(50.f, 150.f);
                window.draw(numberInputText);
            } else {
                window.draw(playerNamesText);
                for (size_t i = 0; i < playerNames.size(); ++i) {
                    sf::Text playerNameText(playerNames[i], font, 24);
                    playerNameText.setPosition(50.f, 200.f + i * 40.f);
                    window.draw(playerNameText);
                }
                if (!inputCompleted) {
                    sf::Text nameInputText(currentInput, font, 24);
                    nameInputText.setPosition(50.f, 200.f + playerNames.size() * 40.f);
                    window.draw(nameInputText);
                }
            }
        } else {
            if (!gameStarted) {
                for (const auto& name : playerNames) {
                    addPlayer(std::unique_ptr<Participant>(new Player(name)));
                }

                std::cout << "Players added: " << playerNames.size() << "\n";
                for (const auto& n : playerNames) std::cout << " - " << n << "\n";

                startRound();
                gameStarted = true;
            }

            update();
            draw(window);
        }

        window.display();
    }
}
