#ifndef MAP_H
#define MAP_H

/** Ao alterar as dimensões da matriz do mapa,
é necessário ajustar esses valores. */
#define mapSizeX 19
#define mapSizeY 19

#include <fstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "brick.h"
#include "pill.h"

enum direction {RIGHT, DOWN, LEFT, UP};
using namespace std;

class Map
{
public:
    Map(void);
    ~Map(void);

    // a melhor solução que encontrei para usar métodos com a matriz do mapa...
    // foi declarando ela como public.
    int tileMap[mapSizeX][mapSizeY];

    // métodos get:
    int getMapSizeX(void);
    int getMapSizeY(void);
    int getScore(void);
    int getHighScore(void);
    int getDotCount(void);
    int getPxSize(void);

    // métodos set:
    void setTileMap(int, int, int);
    void setScore(int);
    void setHighScore(void);
    void setDotCount(int);

    // outros métodos:
    bool loadMap(void);
    bool loadFonts(void);
    bool mapCollision(int, int, int);
    void drawStartMap(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP*, bool, bool, int, int);
    void drawActiveMap(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP*, bool, bool, int, int);
    void drawEndMap(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP*, bool, bool, bool, int, int);
    void drawReadyText(void);
    void drawEndText(void);

private:
    const char* map_filename = "./map/map1.txt";
    const char* font1_filename = "./ttf/crackman.ttf";
    const char* font2_filename = "./ttf/arcadepi.ttf";
    int score;
    int highscore;
    int dotCount;
    int pxSize;
    ALLEGRO_FONT* font1;
    ALLEGRO_FONT* font2;

    // (relação de composição entre classes):
    Brick* tiles;
    Pill* dots;
};

#endif // MAP_H
