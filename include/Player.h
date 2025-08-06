#ifndef PLAYER_H
#define PLAYER_H

#include "Participant.h"

class Player : public Participant {
public:
    Player(const std::string& playerName);
    void play() override;
    bool wantsToHit();
};

#endif
