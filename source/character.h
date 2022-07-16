#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <vector>
#include <allegro5/allegro.h>

class Character
{
    public:
        Character(void);
        ~Character(void);
        const char* getCharName(void);
        int getLabel(void);
        int getPX(void);
        int getPY(void);
        int getIntention(void);
        int getLastDir(void);
        bool isActive(void);
        void setPX(int);
        void setPY(int);
        void setMoveSpeed(float);
        void setIntention(int);
        void setLastDir(int);
        void setMovement(int dir);
        void setActive(bool);
        void isMoving(bool);
        void drawCharacter(ALLEGRO_BITMAP*, int _PIXELSIZE);
    protected:
        const char* charName;
        int label;
        std::vector<float> vy; // Vetor de posições y possíveis na spritesheet.
        float x; float y;
        float w; float h;
        float moveSpeed;
        float px; float py;
        int anim; int intention;
        int lastDir; bool moving;
        bool active;
};

#endif // CHARACTER_H
