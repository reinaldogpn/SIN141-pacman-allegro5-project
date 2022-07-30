#include "smartghost.h"

SmartGhost::SmartGhost()
{
    //ctor
}

SmartGhost::~SmartGhost()
{
    //dtor
}

void SmartGhost::moveCharacter(Character* Player, Map& M)
{
    int Xa = this->getPX();
    int Ya = this->getPY();
    int Xb = Player->getPX();
    int Yb = Player->getPY();

//    double dist_atual = calcDistance(Xa, Xb, Ya, Yb);
//    double dist_dir = calcDistance(Xa+1, Xb, Ya, Yb);
//    double dist_esq = calcDistance(Xa-1, Xb, Ya, Yb);
//    double dist_abaixo = calcDistance(Xa, Xb, Ya+1, Yb);
//    double dist_acima = calcDistance(Xa, Xb, Ya-1, Yb);

    /** ALGORITMO BÁSICO DE PERSEGUIÇÃO **/

    if(Xa > Xb) // fantasma à direita do pacman
    {
        if(!M.mapCollision(Xa, Ya, LEFT))
        {
            intention = LEFT;
        }
    }
    else if(Xa < Xb) // fantasma à esquerda do pacman
    {
        if(!M.mapCollision(Xa, Ya, RIGHT))
        {
            intention = RIGHT;
        }
    }

    if(Ya > Yb) // fantasma abaixo do pacman
    {
        if(!M.mapCollision(Xa, Ya, UP))
        {
            intention = UP;
        }
    }
    else if(Ya < Yb) // fantasma acima do pacman
    {
        if(!M.mapCollision(Xa, Ya, DOWN))
        {
            intention = DOWN;
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
