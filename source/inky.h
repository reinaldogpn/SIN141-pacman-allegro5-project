#ifndef INKY_H
#define INKY_H

#include "randomghost.h"

class Inky : public RandomGhost
{
public:
    Inky(void);
    ~Inky(void);
    void resetCharacter(void);
};

#endif // INKY_H
