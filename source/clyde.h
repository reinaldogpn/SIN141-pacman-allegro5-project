#ifndef CLYDE_H
#define CLYDE_H

#include "character.h"

class Clyde : public Character
{
    public:
        Clyde(void);
        ~Clyde(void);
        void resetGhost(void);
};

#endif // CLYDE_H
