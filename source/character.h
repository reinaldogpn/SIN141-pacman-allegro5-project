#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <math.h>
#include <ctime>
#include <random>
#include <allegro5/allegro.h>

#include "map.h"

using std::vector;

class Character
{
public:
    Character(void);
    virtual ~Character(void);
    Character& operator=(const Character&);

    // métodos get:
    int getId(void);
    int getPX(void);
    int getPY(void);
    int getIntention(void);
    int getLastDir(void);
    bool getMovingStat(void);
    bool getActiveStat(void);
    bool getChasingStat(void);
    bool getRandomStat(void);
    bool getFleeingStat(void);

    // métodos set:
    void setMoveSpeed(float);
    void setPX(int);
    void setPY(int);
    void setIntention(int);
    void setLastDir(int);
    void setMovingStat(bool);
    void setActiveStat(bool);
    void setFleeingStat(bool);

    // métodos virtuais:
    virtual void drawCharacter(ALLEGRO_BITMAP*, int);
    virtual void moveCharacter(Character*, Map&)=0;
    virtual void deathAnimation(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP*, Map&, bool, bool, int, int);
    virtual void resetCharacter(void)=0;
    virtual void switchFleeingMode(bool);

    // outros métodos:
    void deployCharacter(void);
    bool collision(Character*);
    int eatAction(Map&);
    double calcDistance(int, int, int, int);
    void switchCharStats(bool _chasing, bool _random);

protected:
    int id;
    vector<float> vy; // Vetor de posições y possíveis na spritesheet.
    float x;
    float y;
    float w;
    float h;
    float moveSpeed;
    float px;
    float py;
    int anim;
    int intention;
    int lastDir;
    bool moving;
    bool active;
    bool chasing;
    bool random;
    bool fleeing; // não implementado ainda
};

#endif // CHARACTER_H
