#ifndef BLINKY_H
#define BLINKY_H

#include "character.h"

class Blinky : public Character
{
    public:
        Blinky(void);
        ~Blinky(void);
        void resetGhost(void);
};

#endif // BLINKY_H
