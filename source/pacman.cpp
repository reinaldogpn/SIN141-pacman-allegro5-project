#include "pacman.h"

Pacman::Pacman(void)
{
    id = 10;
    vy = {55, 105, 205, 255, 355, 405, 505, 555, 5};
    x = 853;
    y = vy[8];
    w = h = 33;
    moveSpeed = 0.25;
    px = 9;
    py = 13;
    anim = 2;
    intention = 0;
    lastDir = 0;
    moving = true;
    active = true;
    fleeing = chasing = random = false;

}

Pacman::~Pacman(void)
{
    //dtor
}

void Pacman::drawCharacter(ALLEGRO_BITMAP* spriteSheet, int pxSize)
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
                anim = 2;
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
                anim = 2;
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
                anim = 2;
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
                anim = 2;
        }
        else if(anim == 2)
        {
            y = vy[8];
            anim = 0;
        }
        break;
    }

    al_draw_bitmap_region(spriteSheet, x, y, w, h, this->getPX()*pxSize+4, this->getPY()*pxSize+3, 0);
}

void Pacman::moveCharacter(Character*, Map& M)
{
    moving = true;

    int mov = -1;

    if(!M.mapCollision(this->getPX(), this->getPY(), intention))
    {
        mov = intention;
        lastDir = intention;
    }
    else if(!M.mapCollision(this->getPX(), this->getPY(), lastDir))
    {
        mov = lastDir;
    }
    else
        moving = false;

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

    switch(mov)
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

void Pacman::deathAnimation(ALLEGRO_DISPLAY* display, ALLEGRO_BITMAP* spritesheet, Map& M, bool fullscreen, bool mute, int SCREEN_W, int SCREEN_H)
{
    float y = 8;

    // 11 sprites são usadas para essa animação.
    for(int i=0; i<11; i++)
    {
        M.drawActiveMap(display, spritesheet, fullscreen, mute, SCREEN_W, SCREEN_H);
        al_draw_bitmap_region(spritesheet, 351, y, 37, 40, this->getPX()*M.getPxSize()+4, this->getPY()*M.getPxSize()+3, 0);
        al_flip_display();
        al_rest(0.12);
        y+=50;
    }
}

void Pacman::resetCharacter(void)
{
    Pacman newOne;
    *this = newOne;
}

