#include "clyde.h"

using namespace std;

Clyde::Clyde()
{
    charName = "Clyde";
    label = 14;
    x = 151;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.25;
    px = 10;
    py = 8;
    anim = 0;
    intention = 2;
    lastDir = 2;
    moving = false;
    active = false;
//    cout << "Clyde: construct padrao" << endl;
}

Clyde::~Clyde()
{
    //dtor
//    cout << "Clyde: destr" << endl;
}

void Clyde::resetGhost(void)
{
    Clyde newGhost;

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
