#ifndef HAND_H
#define HAND_H

#include <vector>
#include <string>
#include "Card.h"

class Hand {
private:
    std::vector<Card> cards;

public:
    void addCard(Card card);
    int getValue() const;
    void clear();
    std::string toString() const;

    const std::vector<Card>& getCards() const;
};

#endif
