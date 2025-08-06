#ifndef DEALER_H
#define DEALER_H

#include "Participant.h"

class Dealer : public Participant {
public:
    Dealer();
    void play() override;
};

#endif
