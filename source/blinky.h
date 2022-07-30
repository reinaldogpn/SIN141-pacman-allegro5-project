#ifndef BLINKY_H
#define BLINKY_H

#include "smartghost.h"

class Blinky : public SmartGhost
{
public:
    Blinky(void);
    ~Blinky(void);
    void resetCharacter(void);
};

#endif // BLINKY_H
