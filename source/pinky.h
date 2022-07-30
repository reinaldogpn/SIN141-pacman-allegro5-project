#ifndef PINKY_H
#define PINKY_H

#include "smartghost.h"

class Pinky : public SmartGhost
{
public:
    Pinky(void);
    ~Pinky(void);
    void resetCharacter(void);
};

#endif // PINKY_H
