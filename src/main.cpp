#include "../include/BlackjackGame.h"
#include "../include/Player.h"
#include <iostream>
#include <string>

int main() {
    BlackjackGame* game = new BlackjackGame();
    
    game->initGame();
    
    int numPlayers;
    std::cout << "Enter number of players: ";
    std::cin >> numPlayers;
    
    for (int i = 0; i < numPlayers; ++i) {
        std::string playerName;
        std::cout << "Enter player " << (i + 1) << " name: ";
        std::cin >> playerName;
        
        Player* player = new Player(playerName);
        game->addPlayer(player);
    }
    
    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        game->playRound();
        
        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;
    }
    
    delete game;
    return 0;
}
