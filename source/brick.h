#ifndef BRICK_H
#define BRICK_H

#include <iostream>

class Brick
{
    public:
        Brick(void);
        ~Brick(void);
        float getBX(void);
        float getBY(void);
        float getBW(void);
        float getBH(void);
    private:
        float BX; float BY;
        float BW; float BH;
};

#endif // BRICK_H
