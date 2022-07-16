#ifndef PACMAN_H
#define PACMAN_H

#include "character.h"

class Pacman : public Character
{
    public:
        Pacman(void);
        ~Pacman(void);
        void resetPacman(void);
};

#endif // PACMAN_H
