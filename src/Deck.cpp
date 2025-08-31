#include "../include/Deck.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <stdexcept>

Deck::Deck() {
    reset();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::dealCard() {
    if (cards.empty()) {
        reset();
        if (cards.empty()) {
            throw std::runtime_error("Deck::dealCard(): no cards available after reset()");
        }
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

std::size_t Deck::remaining() const {
    return cards.size();
}

void Deck::reset() {
    cards.clear();
    
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            cards.push_back(Card(static_cast<Rank>(rank), static_cast<Suit>(suit)));
        }
    }
    
    shuffle();
}
