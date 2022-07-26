#ifndef PINKY_H
#define PINKY_H

#include "character.h"

class Pinky : public Character
{
public:
    Pinky(void);
    ~Pinky(void);
    void moveCharacter(Character*, Map&);
    void resetCharacter(void);
};

#endif // PINKY_H
