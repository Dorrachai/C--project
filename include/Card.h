#ifndef CARD_H
#define CARD_H

#include <string>

enum class Rank {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
};

enum class Suit {
    HEARTS, DIAMONDS, CLUBS, SPADES
};

class Card {
private:
    Rank rank;
    Suit suit;
    int value;

public:
    Card(Rank r, Suit s);
    int getValue() const;
    std::string toString() const;
};

#endif
