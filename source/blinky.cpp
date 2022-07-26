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

void Blinky::drawCharacter(ALLEGRO_BITMAP* spriteSheet, int pxSize)
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

void Blinky::moveCharacter(Character* Player, Map& M)
{
    int Xa = this->getPX();
    int Ya = this->getPY();
    int Xb = Player->getPX();
    int Yb = Player->getPY();

    vector<int> vDir{0};

    double dist_atual = calcDistance(Xa, Xb, Ya, Yb);
    double dist_dir = calcDistance(Xa+1, Xb, Ya, Yb);
    double dist_esq = calcDistance(Xa-1, Xb, Ya, Yb);
    double dist_abaixo = calcDistance(Xa, Xb, Ya+1, Yb);
    double dist_acima = calcDistance(Xa, Xb, Ya-1, Yb);

    /** ALGORITMO BÁSICO DE PERSEGUIÇÃO **/

    if(Xa > Xb) // fantasma à direita do pacman
    {
        if(!M.mapCollision(Xa, Ya, LEFT))
        {
            intention = LEFT;
        }
        else
        {
            if(dist_acima < dist_abaixo && dist_acima < dist_atual && !M.mapCollision(Xa, Ya, UP))
            {
                intention = UP;
            }
            else if(dist_abaixo < dist_acima && dist_abaixo < dist_atual && !M.mapCollision(Xa, Ya, DOWN))
            {
                intention = DOWN;
            }
        }
    }
    else if(Xa < Xb) // fantasma à esquerda do pacman
    {
        if(!M.mapCollision(Xa, Ya, RIGHT))
        {
            intention = RIGHT;
        }
        else
        {
            if(dist_acima < dist_abaixo && dist_acima < dist_atual && !M.mapCollision(Xa, Ya, UP))
            {
                intention = UP;
            }
            else if(dist_abaixo < dist_acima && dist_abaixo < dist_atual && !M.mapCollision(Xa, Ya, DOWN))
            {
                intention = DOWN;
            }
        }
    }

    if(Ya > Yb) // fantasma abaixo do pacman
    {
        if(!M.mapCollision(Xa, Ya, UP))
        {
            intention = UP;
        }
        else
        {
            if(dist_esq < dist_dir && dist_esq < dist_atual && !M.mapCollision(Xa, Ya, LEFT))
            {
                intention = LEFT;
            }
            else if(dist_dir < dist_esq && dist_dir < dist_abaixo && !M.mapCollision(Xa, Ya, RIGHT))
            {
                intention = RIGHT;
            }
        }
    }
    else if(Ya < Yb) // fantasma acima do pacman
    {
        if(!M.mapCollision(Xa, Ya, DOWN))
        {
            intention = DOWN;
        }
        else
        {
            if(dist_esq < dist_dir && dist_esq < dist_atual && !M.mapCollision(Xa, Ya, LEFT))
            {
                intention = LEFT;
            }
            else if(dist_dir < dist_esq && dist_dir < dist_abaixo && !M.mapCollision(Xa, Ya, RIGHT))
            {
                intention = RIGHT;
            }
        }
    }

    moving = true;

    switch(intention)
    {
    case RIGHT: // RIGHT
        if(!M.mapCollision(this->getPX(), this->getPY(), intention))
        {
            px += moveSpeed;
            lastDir = intention;
        }
        break;
    case DOWN: // DOWN
        if(!M.mapCollision(this->getPX(), this->getPY(), intention))
        {
            py += moveSpeed;
            lastDir = intention;
        }
        break;
    case LEFT: // LEFT
        if(!M.mapCollision(this->getPX(), this->getPY(), intention))
        {
            px -= moveSpeed;
            lastDir = intention;
        }
        break;
    case UP: // UP
        if(!M.mapCollision(this->getPX(), this->getPY(), intention))
        {
            py -= moveSpeed;
            lastDir = intention;
        }
        break;
    }

    // Checa se o personagem está no túnel direito, se sim, teleporta para a outra extremidade do mapa.
    if(this->getPX() == M.getMapSizeX()-1 && this->getPY() == 8)
    {
        this->px = 1;
    }
    // Checa se o personagem está no túnel esquerdo, se sim, teleporta para a outra extremidade do mapa.
    else if(this->getPX() == 0 && this->getPY() == 8)
    {
        this->px = M.getMapSizeX()-2;
    }

    moving = false;
}

void Blinky::resetCharacter(void)
{
    Blinky newOne;
    *this = newOne;
}
