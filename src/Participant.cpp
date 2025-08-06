#include "../include/Participant.h"

Participant::Participant(const std::string& participantName) : name(participantName) {}

Participant::~Participant() {}

int Participant::getHandValue() const {
    return hand.getValue();
}

void Participant::addCard(Card* card) {
    hand.addCard(*card);
}

void Participant::clearHand() {
    hand.clear();
}

std::string Participant::getName() const {
    return name;
}

Hand* Participant::getHand() {
    return &hand;
}
