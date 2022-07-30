#include "pinky.h"

Pinky::Pinky(void)
{
    id = 12;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    x = 51;
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.20;
    px = 9;
    py = 8;
    anim = 0;
    intention = 0;
    lastDir = 3;
    moving = false;
    active = false;
    fleeing = false;
    chasing = true;
    random = false;
}

Pinky::~Pinky(void)
{
    //dtor
}

void Pinky::resetCharacter(void)
{
    Pinky newOne;
    *this = newOne;
}
