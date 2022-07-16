#include "pacman.h"

using namespace std;

Pacman::Pacman(void)
{
    charName = "Pacman";
    label = 10;
    x = 853;
    vy = {55, 105, 205, 255, 355, 405, 505, 555, 5};
    y = vy[8];
    w = h = 33;
    moveSpeed = 0.25;
    px = 9;
    py = 13;
    anim = 2;
    intention = 0;
    moving = true;
    active = true;
//    cout << "pacman: construct padrao" << endl;
}

Pacman::~Pacman(void)
{
    //dtor
//    cout << "pacman: destr" << endl;
}

void Pacman::resetPacman(void)
{
    Pacman newChar;

    y = newChar.y;
    px = newChar.px;
    py = newChar.py;
    anim = newChar.anim;
    intention = newChar.intention;
    moving = newChar.intention;
    active = newChar.active;
}
