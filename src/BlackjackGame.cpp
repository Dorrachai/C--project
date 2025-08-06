#include "../include/BlackjackGame.h"
#include "../include/Player.h"
#include <iostream>

BlackjackGame::BlackjackGame() {
    dealer = new Dealer();
}

BlackjackGame::~BlackjackGame() {
    delete dealer;
    for (Participant* player : players) {
        delete player;
    }
}

void BlackjackGame::initGame() {
    std::cout << "Blackjack Game Started" << std::endl;
    deck.shuffle();
}

void BlackjackGame::playRound() {
    std::cout << "\n=== NEW ROUND ===" << std::endl;
    
    for (Participant* player : players) {
        player->clearHand();
    }
    dealer->clearHand();
    
    std::cout << "Dealing initial cards..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        for (Participant* player : players) {
            Card card = deck.dealCard();
            player->addCard(&card);
        }
        Card dealerCard = deck.dealCard();
        dealer->addCard(&dealerCard);
    }
    
    std::cout << "\nInitial hands:" << std::endl;
    for (Participant* player : players) {
        std::cout << player->getName() << "'s hand: " << player->getHand()->toString() << std::endl;
    }
    std::cout << "Dealer's hand: " << dealer->getHand()->toString() << std::endl;
    
    for (Participant* participant : players) {
        Player* player = static_cast<Player*>(participant);
        player->play();
        
        while (player->wantsToHit() && player->getHandValue() < 21) {
            Card card = deck.dealCard();
            player->addCard(&card);
            std::cout << player->getName() << " draws: " << card.toString() << std::endl;
            std::cout << player->getName() << "'s hand: " << player->getHand()->toString() << std::endl;
            
            if (player->getHandValue() > 21) {
                std::cout << player->getName() << " busts!" << std::endl;
                break;
            }
        }
        
        if (player->getHandValue() <= 21) {
            std::cout << player->getName() << " stands with " << player->getHandValue() << std::endl;
        }
    }
    
    std::cout << "\nDealer's turn:" << std::endl;
    dealer->play();
    
    while (dealer->getHandValue() < 17) {
        Card card = deck.dealCard();
        dealer->addCard(&card);
        std::cout << "Dealer draws: " << card.toString() << std::endl;
        std::cout << "Dealer's hand: " << dealer->getHand()->toString() << std::endl;
    }
    
    if (dealer->getHandValue() > 21) {
        std::cout << "Dealer busts!" << std::endl;
    } else {
        std::cout << "Dealer stands with " << dealer->getHandValue() << std::endl;
    }
    
    std::cout << "\n=== RESULTS ===" << std::endl;
    for (Participant* player : players) {
        int playerValue = player->getHandValue();
        int dealerValue = dealer->getHandValue();
        
        if (playerValue > 21) {
            std::cout << player->getName() << " loses (bust)" << std::endl;
        } else if (dealerValue > 21) {
            std::cout << player->getName() << " wins (dealer bust)" << std::endl;
        } else if (playerValue > dealerValue) {
            std::cout << player->getName() << " wins (" << playerValue << " vs " << dealerValue << ")" << std::endl;
        } else if (playerValue < dealerValue) {
            std::cout << player->getName() << " loses (" << playerValue << " vs " << dealerValue << ")" << std::endl;
        } else {
            std::cout << player->getName() << " ties (" << playerValue << " vs " << dealerValue << ")" << std::endl;
        }
    }
}

void BlackjackGame::addPlayer(Participant* player) {
    players.push_back(player);
}
