#include "../include/Dealer.h"
#include <iostream>

Dealer::Dealer() : Participant("Dealer") {}

void Dealer::play() {
    std::cout << "Dealer's hand: " << getHand()->toString() << std::endl;
    
    while (getHand()->getValue() < 17) {
        std::cout << "Dealer hits" << std::endl;
        break;
    }
    
    if (getHand()->getValue() >= 17) {
        std::cout << "Dealer stands" << std::endl;
    }
}
