#include "character.h"

Character::Character(void)
{
    //ctor
}

Character::~Character(void)
{
    //dtor
}

Character& Character::operator=(const Character& other)
{
    if(this == &other)
        return *this;

    this->id = other.id;
    this->vy = other.vy;
    this->x = other.x;
    this->y = other.y;
    this->w = other.w;
    this->h = other.h;
    this->moveSpeed = other.moveSpeed;
    this->px = other.px;
    this->py = other.py;
    this->anim = other.anim;
    this->intention = other.intention;
    this->lastDir = other.lastDir;
    this->moving = other.moving;
    this->active = other.active;
    this->fleeing = other.fleeing;
    this->chasing = other.chasing;
    this->random = other.random;

    return *this;
}

int Character::getId(void)
{
    return id;
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

bool Character::getMovingStat(void)
{
    return moving;
}

bool Character::getActiveStat(void)
{
    return active;
}

bool Character::getChasingStat(void)
{
    return chasing;
}

bool Character::getRandomStat(void)
{
    return random;
}

bool Character::getFleeingStat(void)
{
    return fleeing;
}

void Character::setMoveSpeed(float _moveSpeed)
{
    moveSpeed = _moveSpeed;
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

void Character::setMovingStat(bool _moving)
{
    moving = _moving;
}

void Character::setActiveStat(bool _active)
{
    active = _active;
}

void Character::setFleeingStat(bool _fleeing)
{
    fleeing = _fleeing;
}

void Character::drawCharacter(ALLEGRO_BITMAP* spriteSheet, int pxSize)
{
    if(active)
    {
        switch(intention)
        {
        case 0: // RIGHT
            if(anim == 0)
            {
                y = vy[0];
                anim = 1;
            }
            else if(anim == 1)
            {
                y = vy[1];
                anim = 0;
            }
            break;
        case 1: // DOWN
            if(anim == 0)
            {
                y = vy[2];
                anim = 1;
            }
            else if(anim == 1)
            {
                y = vy[3];
                anim = 0;
            }
            break;
        case 2: // LEFT
            if(anim == 0)
            {
                y = vy[4];
                anim = 1;
            }
            else if(anim == 1)
            {
                y = vy[5];
                anim = 0;
            }
            break;
        case 3: // UP
            if(anim == 0)
            {
                y = vy[6];
                anim = 1;
            }
            else if(anim == 1)
            {
                y = vy[7];
                anim = 0;
            }
            break;
        }
    }

    al_draw_bitmap_region(spriteSheet, x, y, w, h, this->getPX()*pxSize+4, this->getPY()*pxSize+3, 0);
}

void Character::deathAnimation(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP*, Map&, bool, bool, int, int)
{
    // ...
}

void Character::deployCharacter(void)
{
    this->px = 9;
    this->py = 6;
}

bool Character::collision(Character* other)
{
    if(this->getPX() != other->getPX() || this->getPY() != other->getPY())
        return false;
    else
        return true;
}

int Character::eatAction(Map& M)
{
//     Se o player "comer" uma pílula ou super pílula... esvazia a célula, incrementa o placar e decrementa o contador de pílulas.
    if(M.tileMap[this->getPX()][this->getPY()] == 2)
    {
        M.setTileMap(this->getPX(), this->getPY(), 0);
        M.setScore(M.getScore()+1);
        M.setDotCount(-1);

        return 1;
    }
    else if(M.tileMap[this->getPX()][this->getPY()] == 3)
    {
        M.setTileMap(this->getPX(), this->getPY(), 0);
        M.setScore(M.getScore()+100);
        M.setDotCount(-1);

        return 2;
    }

    return 0;
}

double Character::calcDistance(int Xa, int Ya, int Xb, int Yb)
{
    // Fórmula para calcular a distância entre dois pontos no plano:
    return abs(sqrt(pow(Xb-Xa, 2)+pow(Yb-Ya, 2)));
}

void Character::switchCharStats(bool _chasing, bool _random)
{
    chasing = _chasing;
    random = _random;
}

void Character::switchFleeingMode(bool)
{
    // ...
}
