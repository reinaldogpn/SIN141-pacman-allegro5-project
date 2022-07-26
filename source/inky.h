#ifndef INKY_H
#define INKY_H

#include "character.h"

class Inky : public Character
{
public:
    Inky(void);
    ~Inky(void);
    void resetCharacter(void);
};

#endif // INKY_H
