#ifndef RANDOMGHOST_H
#define RANDOMGHOST_H

#include "character.h"

class RandomGhost : public Character
{
    public:
        RandomGhost();
        ~RandomGhost();
        virtual void moveCharacter(Character*, Map&);
};

#endif // RANDOMGHOST_H
