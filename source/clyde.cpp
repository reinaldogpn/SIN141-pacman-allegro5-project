#include "clyde.h"

Clyde::Clyde(void)
{
    id = 14;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    x = 151;
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.20;
    px = 10;
    py = 8;
    anim = 0;
    intention = 2;
    lastDir = 2;
    moving = false;
    active = false;
    fleeing = false;
    chasing = false;
    random = true;
}

Clyde::~Clyde(void)
{
    //dtor
}

void Clyde::resetCharacter(void)
{
    Clyde newOne;
    *this = newOne;
}
