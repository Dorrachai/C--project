#include "../include/Player.h"
#include <iostream>

Player::Player(const std::string& playerName) : Participant(playerName) {}

void Player::play() {
    std::cout << name << "'s turn:" << std::endl;
    std::cout << name << "'s hand: " << hand.toString() << std::endl;
}

bool Player::wantsToHit() {
    if (hand.getValue() >= 21) {
        return false;
    }
    
    char choice;
    std::cout << name << ", hit or stand? (h/s): ";
    std::cin >> choice;
    
    return (choice == 'h' || choice == 'H');
}
