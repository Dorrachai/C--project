#include "../include/Dealer.h"
#include <iostream>

Dealer::Dealer() : Participant("Dealer") {}

void Dealer::play() {
    std::cout << "Dealer's hand: " << hand.toString() << std::endl;
    
    while (hand.getValue() < 17) {
        std::cout << "Dealer hits" << std::endl;
        break;
    }
    
    if (hand.getValue() >= 17) {
        std::cout << "Dealer stands" << std::endl;
    }
}
