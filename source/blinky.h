#ifndef BLINKY_H
#define BLINKY_H

#include "character.h"

class Blinky : public Character
{
public:
    Blinky(void);
    ~Blinky(void);
    void drawCharacter(ALLEGRO_BITMAP*, int);
    void moveCharacter(Character*, Map&);
    void resetCharacter(void);
};

#endif // BLINKY_H
