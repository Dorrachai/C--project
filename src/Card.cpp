#include "../include/Card.h"

Card::Card(Rank r, Suit s) : rank(r), suit(s) {
    if (rank == Rank::ACE) {
        value = 11;
    } else if (rank >= Rank::JACK) {
        value = 10;
    } else {
        value = static_cast<int>(rank);
    }
}

int Card::getValue() const {
    return value;
}

std::string Card::toString() const {
    std::string rankStr;
    switch (rank) {
        case Rank::ACE: rankStr = "A"; break;
        case Rank::TWO: rankStr = "2"; break;
        case Rank::THREE: rankStr = "3"; break;
        case Rank::FOUR: rankStr = "4"; break;
        case Rank::FIVE: rankStr = "5"; break;
        case Rank::SIX: rankStr = "6"; break;
        case Rank::SEVEN: rankStr = "7"; break;
        case Rank::EIGHT: rankStr = "8"; break;
        case Rank::NINE: rankStr = "9"; break;
        case Rank::TEN: rankStr = "10"; break;
        case Rank::JACK: rankStr = "J"; break;
        case Rank::QUEEN: rankStr = "Q"; break;
        case Rank::KING: rankStr = "K"; break;
    }
    
    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS: suitStr = "H"; break;
        case Suit::DIAMONDS: suitStr = "D"; break;
        case Suit::CLUBS: suitStr = "C"; break;
        case Suit::SPADES: suitStr = "S"; break;
    }
    
    return rankStr + suitStr;
}
