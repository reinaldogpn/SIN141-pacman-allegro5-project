#include "inky.h"

using namespace std;

Inky::Inky()
{
    charName = "Inky";
    label = 13;
    x = 101;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.25;
    px = 8;
    py = 8;
    anim = 0;
    intention = 0;
    lastDir = 0;
    moving = false;
    active = false;
//    cout << "Inky: construct padrao" << endl;
}

Inky::~Inky()
{
    //dtor
//    cout << "Inky: destr" << endl;
}

void Inky::resetGhost(void)
{
    Inky newGhost;

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
