#include <SFML/Graphics.hpp>
#include "../include/BlackjackGame.h"
#include "../include/Player.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Blackjack");
    window.setFramerateLimit(60);

    BlackjackGame* game = new BlackjackGame();
    game->initGame();

    // Font för input UI
    sf::Font font;
    if (!font.loadFromFile("font/comic.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
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
    int currentPlayerIndex = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (!inputCompleted) {
                if (event.type == sf::Event::TextEntered) {
                    if (inputNumberOfPlayers) {
                        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                            currentInput += static_cast<char>(event.text.unicode);
                        } else if (event.text.unicode == '\b' && !currentInput.empty()) {
                            currentInput.pop_back();
                        } else if (event.text.unicode == '\r' && !currentInput.empty()) {
                            numPlayers = std::stoi(currentInput);
                            currentInput.clear();
                            inputNumberOfPlayers = false;
                            std::cout << "Number of players: " << numPlayers << std::endl;
                        }
                    } else {
                        if (event.text.unicode == '\b' && !currentInput.empty()) {
                            currentInput.pop_back();
                        } else if (event.text.unicode == '\r' && !currentInput.empty()) {
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
                game->handleEvent(event, window);

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    game->startRound();
                }
            }
        }

        window.clear(sf::Color::Green);
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
                    game->addPlayer(new Player(name));
                }

                // Debug: skriv ut vilka som lades till
                std::cout << "Players added: " << playerNames.size() << "\n";
                for (const auto& n : playerNames) std::cout << " - " << n << "\n";

                game->startRound();   // ✅ nu finns players → PlayerTurn
                gameStarted = true;
            }

            // Vanlig spel-loop
            game->update();
            game->draw(window);
        }

window.display();
    }

    delete game;
    return 0;
}
