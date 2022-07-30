#include "randomghost.h"

RandomGhost::RandomGhost()
{
    //ctor
}

RandomGhost::~RandomGhost()
{
    //dtor
}

void RandomGhost::moveCharacter(Character*, Map& M)
{
    int sel_dir, random, mov = -1, crossing_type = 0;
    bool crossing = false;
    vector<int> vDir{0}; // Vector de possíveis direções.

    // Encruzilhada 1: todas as direções possíveis
    if(M.tileMap[this->getPX()+1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()+1] != 1 && M.tileMap[this->getPX()-1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()-1] != 1)
    {
        crossing = true;
        crossing_type = 1;
    }
    // Encruzilhada 2: todas as direções possíveis exceto cima
    else if(M.tileMap[this->getPX()+1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()+1] != 1 && M.tileMap[this->getPX()-1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()-1] == 1)
    {
        crossing = true;
        crossing_type = 2;
    }
    // Encruzilhada 3: todas as direções possíveis exceto esq.
    else if(M.tileMap[this->getPX()+1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()+1] != 1 && M.tileMap[this->getPX()][this->getPY()-1] != 1 && M.tileMap[this->getPX()-1][this->getPY()] == 1)
    {
        crossing = true;
        crossing_type = 3;
    }
    // Encruzilhada 4: todas as direções possíveis exceto dir.
    else if(M.tileMap[this->getPX()][this->getPY()+1] != 1 && M.tileMap[this->getPX()-1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()-1] != 1 && M.tileMap[this->getPX()+1][this->getPY()] == 1)
    {
        crossing = true;
        crossing_type = 4;
    }
    // Encruzilhada 5: todas direções possíveis exceto baixo
    else if(M.tileMap[this->getPX()+1][this->getPY()] != 1 && M.tileMap[this->getPX()-1][this->getPY()] != 1 && M.tileMap[this->getPX()][this->getPY()-1] != 1 && M.tileMap[this->getPX()][this->getPY()+1] == 1)
    {
        crossing = true;
        crossing_type = 5;
    }

    if(crossing)
    {
        srand(time(nullptr));

        switch(crossing_type)
        {
        case 1:
            sel_dir = rand()%4;
            intention = sel_dir;
            break;
        case 2:
            vDir = {RIGHT, DOWN, LEFT};
            random = rand()%vDir.size();
            sel_dir = vDir[random];
            intention = sel_dir;
            break;
        case 3:
            vDir = {UP, DOWN, RIGHT};
            random = rand()%vDir.size();
            sel_dir = vDir[random];
            intention = sel_dir;
            break;
        case 4:
            vDir = {DOWN, LEFT, UP};
            random = rand()%vDir.size();
            sel_dir = vDir[random];
            intention = sel_dir;
            break;
        case 5:
            vDir = {RIGHT, LEFT, UP};
            random = rand()%vDir.size();
            sel_dir = vDir[random];
            intention = sel_dir;
            break;
        }
    }

    if(!M.mapCollision(this->getPX(), this->getPY(), intention))
    {
        mov = intention;
    }
    else
    {
        vDir = {RIGHT, LEFT, UP, DOWN};
        random = rand()%vDir.size();
        sel_dir = vDir[random];

        do
        {
            random = rand()%vDir.size();
            sel_dir = vDir[random];
        }
        while(sel_dir == intention);

        intention = sel_dir;
        mov = intention;
    }

    switch(mov)
    {
    case RIGHT: // RIGHT
        if(!M.mapCollision(this->getPX(), this->getPY(), mov))
            px += moveSpeed;
        break;
    case DOWN: // DOWN
        if(!M.mapCollision(this->getPX(), this->getPY(), mov))
            py += moveSpeed;
        break;
    case LEFT: // LEFT
        if(!M.mapCollision(this->getPX(), this->getPY(), mov))
            px -= moveSpeed;
        break;
    case UP: // UP
        if(!M.mapCollision(this->getPX(), this->getPY(), mov))
            py -= moveSpeed;
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

    lastDir = mov;
    moving = crossing = false;
}
