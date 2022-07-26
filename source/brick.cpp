#include "brick.h"

Brick::Brick(void)
{
    BX = 721;
    BY = 772;
    BW = BH = 40;
}

Brick::~Brick(void)
{
    //dtor
}

float Brick::getBX(void)
{
    return BX;
}

float Brick::getBY(void)
{
    return BY;
}

float Brick::getBW(void)
{
    return BW;
}

float Brick::getBH(void)
{
    return BH;
}
