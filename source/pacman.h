#ifndef PACMAN_H
#define PACMAN_H

#include "character.h"

class Pacman : public Character
{
public:
    Pacman(void);
    ~Pacman(void);
    void drawCharacter(ALLEGRO_BITMAP*, int);
    virtual void moveCharacter(Character*, Map&);
    void deathAnimation(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP*, Map&, bool, bool, int, int);
    void resetCharacter(void);
};

#endif // PACMAN_H
