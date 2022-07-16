#include "character.h"

using namespace std;

Character::Character(void)
{
    //ctor
//    cout << "character: construct padrao" << endl;
}

Character::~Character(void)
{
    //dtor
//    cout << "character: destr" << endl;
}

const char* Character::getCharName(void)
{
    return charName;
}

int Character::getLabel(void)
{
    return label;
}

int Character::getPX(void)
{
    return px;
}

int Character::getPY(void)
{
    return py;
}

int Character::getIntention(void)
{
    return intention;
}

int Character::getLastDir(void)
{
    return lastDir;
}

bool Character::isActive(void)
{
    return active;
}

void Character::setPX(int _px)
{
    px = _px;
}

void Character::setPY(int _py)
{
    py = _py;
}

void Character::setIntention(int _intention)
{
    intention = _intention;
}

void Character::setLastDir(int _lastDir)
{
    lastDir = _lastDir;
}

void Character::setMoveSpeed(float _moveSpeed)
{
    moveSpeed = _moveSpeed;
}

void Character::setMovement(int dir)
{
    switch(dir)
    {
        case 0: // RIGHT
            px += moveSpeed;
            break;
        case 1: // DOWN
            py += moveSpeed;
            break;
        case 2: // LEFT
            px -= moveSpeed;
            break;
        case 3: // UP
            py -= moveSpeed;
            break;
    }
}

void Character::setActive(bool _active)
{
    active = _active;
}

void Character::isMoving(bool _moving)
{
    moving = _moving;
}

void Character::drawCharacter(ALLEGRO_BITMAP* spriteSheet, int _PIXELSIZE)
{
    switch(lastDir)
    {
    case 0: // RIGHT
        if(anim == 0)
        {
            y = vy[0];
            if(moving)
                anim = 1;
        }
        else if(anim == 1)
        {
            y = vy[1];
            if(moving) // O pacman tem uma animação a mais...
                (label == 10)? anim = 2: anim = 0;
        }
        else if(anim == 2)
        {
            y = vy[8];
            anim = 0;
        }
        break;
    case 1: // DOWN
        if(anim == 0)
        {
            y = vy[2];
            if(moving)
                anim = 1;
        }
        else if(anim == 1)
        {
            y = vy[3];
            if(moving)
                (label == 10)? anim = 2: anim = 0;
        }
        else if(anim == 2)
        {
            y = vy[8];
            anim = 0;
        }
        break;
    case 2: // LEFT
        if(anim == 0)
        {
            y = vy[4];
            if(moving)
                anim = 1;
        }
        else if(anim == 1)
        {
            y = vy[5];
            if(moving)
                (label == 10)? anim = 2: anim = 0;
        }
        else if(anim == 2)
        {
            y = vy[8];
            anim = 0;
        }
        break;
    case 3: // UP
        if(anim == 0)
        {
            y = vy[6];
            if(moving)
                anim = 1;
        }
        else if(anim == 1)
        {
            y = vy[7];
            if(moving)
                (label == 10)? anim = 2: anim = 0;
        }
        else if(anim == 2)
        {
            y = vy[8];
            anim = 0;
        }
        break;
    }

    al_draw_bitmap_region(spriteSheet, x, y, w, h, (int)px*_PIXELSIZE+4, (int)py*_PIXELSIZE+3, 0);
}
