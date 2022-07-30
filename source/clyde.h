#ifndef CLYDE_H
#define CLYDE_H

#include "randomghost.h"

class Clyde : public RandomGhost
{
public:
    Clyde(void);
    ~Clyde(void);
    void resetCharacter(void);
};

#endif // CLYDE_H
