#include "blinky.h"

Blinky::Blinky(void)
{
    id = 11;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    x = 1;
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.20;
    px = 9;
    py = 6;
    anim = 0;
    intention = 0;
    lastDir = 0;
    moving = false;
    active = true;
    fleeing = false;
    chasing = true;
    random = false;
}

Blinky::~Blinky(void)
{
    //dtor
}

void Blinky::resetCharacter(void)
{
    Blinky newOne;
    *this = newOne;
}
