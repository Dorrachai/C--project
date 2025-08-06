#include "../include/Hand.h"

void Hand::addCard(Card card) {
    cards.push_back(card);
}

int Hand::getValue() const {
    int total = 0;
    int aces = 0;
    
    for (const Card& card : cards) {
        total += card.getValue();
        if (card.getValue() == 11) {
            aces++;
        }
    }
    
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    
    return total;
}

void Hand::clear() {
    cards.clear();
}

std::string Hand::toString() const {
    std::string result = "";
    for (size_t i = 0; i < cards.size(); ++i) {
        if (i > 0) result += " ";
        result += cards[i].toString();
    }
    result += " (" + std::to_string(getValue()) + ")";
    return result;
}
