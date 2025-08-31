#include "../include/Player.h"
#include <iostream>

Player::Player(const std::string& playerName) : Participant(playerName) {}

void Player::play() {
    std::cout << getName() << "'s turn:" << std::endl;
    std::cout << getName() << "'s hand: " << getHand()->toString() << std::endl;
}

bool Player::wantsToHit() {
    if (getHand()->getValue() >= 21) {
        return false;
    }
    
    char choice;
    std::cout << getName() << ", hit or stand? (h/s): ";
    std::cin >> choice;
    
    return (choice == 'h' || choice == 'H');
}
