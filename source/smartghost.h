#ifndef SMARTGHOST_H
#define SMARTGHOST_H

#include "character.h"

class SmartGhost : public Character
{
    public:
        SmartGhost();
        ~SmartGhost();
        virtual void moveCharacter(Character*, Map&);
};

#endif // SMARTGHOST_H
