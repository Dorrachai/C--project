#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include "Hand.h"
#include "Card.h"

class Participant {
protected:
    std::string name;
    Hand hand;

public:
    Participant(const std::string& participantName);
    virtual ~Participant();
    
    int getHandValue() const;
    void addCard(const Card& card);
    void clearHand();
    virtual void play() = 0;
    
    std::string getName() const;
    Hand* getHand();
};

#endif
