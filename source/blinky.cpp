#include "blinky.h"

using namespace std;

Blinky::Blinky(void)
{
    charName = "Blinky";
    label = 11;
    x = 1;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.25;
    px = 9;
    py = 6;
    anim = 0;
    intention = 0;
    lastDir = 0;
    moving = false;
    active = true;
//    cout << "blinky: construct padrao" << endl;
}

Blinky::~Blinky(void)
{
    //dtor
//    cout << "blinky: destr" << endl;
}

void Blinky::resetGhost(void)
{
    Blinky newGhost;

    y = newGhost.y;
    moveSpeed = newGhost.moveSpeed;
    px = newGhost.px;
    py = newGhost.py;
    anim = newGhost.anim;
    intention = newGhost.intention;
    lastDir = newGhost.lastDir;
    moving = newGhost.moving;
    active = newGhost.active;
}
