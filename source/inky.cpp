#include "inky.h"

Inky::Inky(void)
{
    id = 13;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    x = 101;
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.20;
    px = 8;
    py = 8;
    anim = 0;
    intention = 0;
    lastDir = 0;
    moving = false;
    active = false;
    fleeing = false;
    chasing = false;
    random = true;
}

Inky::~Inky(void)
{
    //dtor
}

void Inky::resetCharacter(void)
{
    Inky newOne;
    *this = newOne;
}
