#include "pinky.h"

using namespace std;

Pinky::Pinky()
{
    charName = "Pinky";
    label = 12;
    x = 51;
    vy = {4, 54, 104, 154, 204, 254, 304, 354};
    y = vy[0];
    w = h = 35;
    moveSpeed = 0.25;
    px = 9;
    py = 8;
    anim = 0;
    intention = 0;
    lastDir = 3;
    moving = false;
    active = false;
//    cout << "pinky: construct padrao" << endl;
}

Pinky::~Pinky()
{
    //dtor
//    cout << "pinky: destr" << endl;
}

void Pinky::resetGhost(void)
{
    Pinky newGhost;

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
