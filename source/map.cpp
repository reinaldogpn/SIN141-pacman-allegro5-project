#include "map.h"

Map::Map(void)
{
    tiles = new Brick();
    dots = new Pill();

    font1 = NULL;
    font2 = NULL;

    score = 0;
    highscore = 0;
    dotCount = 0;
    pxSize = (tiles->getBW() + tiles->getBH()) / 2;
}

Map::~Map(void)
{
    al_destroy_font(font1);
    al_destroy_font(font2);

    delete tiles;
    delete dots;
}

bool Map::loadFonts(void)
{
    font1 = al_load_font(font1_filename, 60, 0);
    font2 = al_load_font(font2_filename, 14, 0);

    if(!font1 || !font2)
    {
        return false;
    }

    return true;
}

bool Map::mapCollision(int X, int Y, int dir)
{
    switch(dir) // Aqui o programa checa se a próxima posição do personagem é uma parede; se sim, há colisão (retorna true); se não, não há colisão (retorna false).
    {
    case RIGHT:
        if(tileMap[X+1][Y] != 1 && X+1 <= mapSizeX-1)
        {
            return false;
        }
        break;
    case DOWN:
        if(tileMap[X][Y+1] != 1 && Y+1 < mapSizeY-1)
        {
            return false;
        }
        break;
    case LEFT:
        if(tileMap[X-1][Y] != 1 && X-1 >= 0)
        {
            return false;
        }
        break;
    case UP:
        if(tileMap[X][Y-1] != 1 && Y-1 > 0)
        {
            return false;
        }
        break;
    }

    return true;
}

bool Map::loadMap(void)
{
    int contX = 0, contY = 0;
    dotCount = 0;

    ifstream openfile(map_filename);                                            /** Abre o arquivo .txt no qual está contida a matriz usada para criar o mapa do jogo.*/
    if(openfile.is_open())                                                      /** Se o arquivo for aberto com êxito... */
    {
        while(!openfile.eof())                                                  /** Enquanto não chegar no fim do arquivo... */
        {
            openfile >> tileMap[contX][contY];                                  /** Lê os valores da matriz de cima pra baixo, um por um e insere os valores nas respectivas posições da matriz... */

            if(tileMap[contX][contY] == 2 || tileMap[contX][contY] == 3)        /** Lê o número de pílulas que vão haver no mapa e incrementa o contador de pílulas. */
                dotCount++;

            contX++;

            if(contX >= mapSizeX)                                               /** ... quando o contador de linhas (contX) chegar na última linha do arquivo, ele volta para a linha 0 e o ... */
            {
                                                                                /** ... contador de colunas (contY) incrementa +1... */
                contX = 0;
                contY++;
            }                                                                   /** ... e assim o processo é repetido até 'EOF' e a matriz ser completamente preenchida. */
        }                                                                       /** Esse método me permite alterar o mapa posteriormente sem a necessidade de recompilar todo o jogo. */

        openfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void Map::drawStartMap(ALLEGRO_DISPLAY* display, ALLEGRO_BITMAP* spritesheet, bool fullscreen, bool mute, int SCREEN_W, int SCREEN_H)
{
    // Criei essas variaveis para as funçoes de desenhar não ficarem tão poluídas...
    al_clear_to_color(al_map_rgb(0,0,0));

    for(int i=0; i<mapSizeX; i++)
    {
        for(int j=0; j<mapSizeY; j++)
        {
            if(tileMap[i][j] == 1) // Valor na matriz igual a 1 = Tijolo;
            {
                if(i != 9 || j != 7) // Não desenha tijolo na posição da portinha da ghostzone.
                    al_draw_bitmap_region(spritesheet, tiles->getBX(), tiles->getBY(), tiles->getBW(), tiles->getBH(), i*pxSize, j*pxSize, 0);
            }
            else if(tileMap[i][j] == 2) // Valor na matriz igual a 2 = Pílula;
            {
                // Aqui são feitos alguns ajustes para a pílula aparecer exatamente no centro de cada célula do mapa, tal que BX, BY = 40. A expressão fica assim: (i*40)+(20)-1 // (j*40)+(20)-1, sendo i e j as posições na matriz do mapa.
                al_draw_bitmap_region(spritesheet, dots->getPX(), dots->getPY(), dots->getPW(), dots->getPH(), ((i*pxSize)+(pxSize/2)-1), ((j*pxSize)+(pxSize/2)-1), 0);
            }
            else if(tileMap[i][j] == 3) // Valor na matriz igual a 3 = Super pílula;
            {
                al_draw_bitmap_region(spritesheet, 411, 313, 16, 16, ((i*pxSize)+(pxSize/2)-7), ((j*pxSize)+(pxSize/2)-7), 0);
            }

        }

    }

    // Portinha da ghostzone
    al_draw_filled_rectangle(360.5, 7*pxSize+0.5, 360+pxSize, 7*pxSize+6, al_map_rgb(0,18,173));

    // Funçoes que desenham o texto permanente da tela do jogo.
    if(fullscreen)
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (pxSize*mapSizeY)+((SCREEN_W-pxSize*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(font2, al_map_rgb(255,255,255), SCREEN_W-136, SCREEN_H-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(font2, al_map_rgb(255,255,255), SCREEN_W-132, SCREEN_H-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(font2, al_map_rgb(255,241,0), 9*pxSize-8, 8*pxSize+12, 0, "READY!");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "PRESSIONE ENTER PARA INICIAR!");

        // Altera o icone do auto falante que representa o som (mutado ou desmutado).
        (mute)? al_draw_bitmap_region(spritesheet, 780, 797, 20, 20, SCREEN_W-35, SCREEN_H-35, 0) : al_draw_bitmap_region(spritesheet, 780, 773, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
    }
    else
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (pxSize*mapSizeY)+((al_get_display_width(display)-pxSize*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(font2, al_map_rgb(255,255,255), (al_get_display_width(display))-136, (al_get_display_height(display))-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(font2, al_map_rgb(255,255,255), (al_get_display_width(display))-132, (al_get_display_height(display))-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(font2, al_map_rgb(255,241,0), 9*pxSize-8, 8*pxSize+12, 0, "READY!");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "PRESSIONE ENTER PARA INICIAR!");

        // Altera o icone do auto falante que representa o som (mutado ou desmutado).
        (mute)? al_draw_bitmap_region(spritesheet, 780, 797, 20, 20, (al_get_display_width(display))-35, (al_get_display_height(display))-35, 0) :
        al_draw_bitmap_region(spritesheet, 780, 773, 20, 20, (al_get_display_width(display))-35, (al_get_display_height(display))-35, 0);
    }
}

void Map::drawActiveMap(ALLEGRO_DISPLAY* display, ALLEGRO_BITMAP* spritesheet, bool fullscreen, bool mute, int SCREEN_W, int SCREEN_H)
{
    al_clear_to_color(al_map_rgb(0,0,0));

    for(int i=0; i<mapSizeX; i++)
    {
        for(int j=0; j<mapSizeY; j++)
        {
            if(tileMap[i][j] == 1) // Valor na matriz igual a 1 = Tijolo;
            {
                if(i != 9 || j != 7) // Não desenha tijolo na posição da portinha da ghostzone.
                    al_draw_bitmap_region(spritesheet, tiles->getBX(), tiles->getBY(), tiles->getBW(), tiles->getBH(), i*pxSize, j*pxSize, 0);
            }
            else if(tileMap[i][j] == 2) // Valor na matriz igual a 2 = Pílula;
            {
                // Aqui são feitos alguns ajustes para a pílula aparecer exatamente no centro de cada célula do mapa, tal que BX, BY = 40. A expressão fica assim: (i*40)+(20)-1 // (j*40)+(20)-1, sendo i e j as posições na matriz do mapa.
                al_draw_bitmap_region(spritesheet, dots->getPX(), dots->getPY(), dots->getPW(), dots->getPH(), ((i*pxSize)+(pxSize/2)-1), ((j*pxSize)+(pxSize/2)-1), 0);
            }
            else if(tileMap[i][j] == 3) // Valor na matriz igual a 3 = Super pílula;
            {
                al_draw_bitmap_region(spritesheet, 411, 313, 16, 16, ((i*pxSize)+(pxSize/2)-7), ((j*pxSize)+(pxSize/2)-7), 0);
            }

        }

    }

    // Portinha da ghostzone
    al_draw_filled_rectangle(360.5, 7*pxSize+0.5, 360+pxSize, 7*pxSize+6, al_map_rgb(0,18,173));

    // Funçoes que desenham o texto permanente da tela do jogo.
    if(fullscreen)
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (pxSize*mapSizeY)+((SCREEN_W-pxSize*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(font2, al_map_rgb(255,255,255), SCREEN_W-136, SCREEN_H-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(font2, al_map_rgb(255,255,255), SCREEN_W-132, SCREEN_H-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");

        // Altera o icone do auto falante que representa o som (mutado ou desmutado).
        (mute)? al_draw_bitmap_region(spritesheet, 780, 797, 20, 20, SCREEN_W-35, SCREEN_H-35, 0) : al_draw_bitmap_region(spritesheet, 780, 773, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
    }
    else
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (pxSize*mapSizeY)+((al_get_display_width(display)-pxSize*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(font2, al_map_rgb(255,255,255), (al_get_display_width(display))-136, (al_get_display_height(display))-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(font2, al_map_rgb(255,255,255), (al_get_display_width(display))-132, (al_get_display_height(display))-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");

        // Altera o icone do auto falante que representa o som (mutado ou desmutado).
        (mute)? al_draw_bitmap_region(spritesheet, 780, 797, 20, 20, (al_get_display_width(display))-35, (al_get_display_height(display))-35, 0) : al_draw_bitmap_region(spritesheet, 780, 773, 20, 20, (al_get_display_width(display))-35, (al_get_display_height(display))-35, 0);
    }
}

void Map::drawEndMap(ALLEGRO_DISPLAY* display, ALLEGRO_BITMAP* spritesheet, bool fullscreen, bool mute, bool win, int SCREEN_W, int SCREEN_H)
{
    al_clear_to_color(al_map_rgb(0,0,0));

    for(int i=0; i<mapSizeX; i++)
    {
        for(int j=0; j<mapSizeY; j++)
        {
            if(tileMap[i][j] == 1) // Valor na matriz igual a 1 = Tijolo;
            {
                if(i != 9 || j != 7) // Não desenha tijolo na posição da portinha da ghostzone.
                    al_draw_bitmap_region(spritesheet, tiles->getBX(), tiles->getBY(), tiles->getBW(), tiles->getBH(), i*pxSize, j*pxSize, 0);
            }
            else if(tileMap[i][j] == 2) // Valor na matriz igual a 2 = Pílula;
            {
                // Aqui são feitos alguns ajustes para a pílula aparecer exatamente no centro de cada célula do mapa, tal que BX, BY = 40. A expressão fica assim: (i*40)+(20)-1 // (j*40)+(20)-1, sendo i e j as posições na matriz do mapa.
                al_draw_bitmap_region(spritesheet, dots->getPX(), dots->getPY(), dots->getPW(), dots->getPH(), ((i*pxSize)+(pxSize/2)-1), ((j*pxSize)+(pxSize/2)-1), 0);
            }
            else if(tileMap[i][j] == 3) // Valor na matriz igual a 3 = Super pílula;
            {
                al_draw_bitmap_region(spritesheet, 411, 313, 16, 16, ((i*pxSize)+(pxSize/2)-7), ((j*pxSize)+(pxSize/2)-7), 0);
            }

        }

    }

    // Portinha da ghostzone
    al_draw_filled_rectangle(360.5, 7*pxSize+0.5, 360+pxSize, 7*pxSize+6, al_map_rgb(0,18,173));

    // Funçoes que desenham o texto permanente da tela do jogo.
    if(fullscreen)
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (pxSize*mapSizeY)+((SCREEN_W-pxSize*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(font2, al_map_rgb(255,255,255), SCREEN_W-136, SCREEN_H-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(font2, al_map_rgb(255,255,255), SCREEN_W-132, SCREEN_H-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "JOGAR NOVAMENTE?");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x, 320, ALLEGRO_ALIGN_CENTER, "SIM (Y) / NAO (N)");

        (win)? al_draw_text(font2, al_map_rgb(0,255,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "VOCE VENCEU!") : al_draw_text(font2, al_map_rgb(255,0,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "GAME OVER");

        // Altera o icone do auto falante que representa o som (mutado ou desmutado).
        (mute)? al_draw_bitmap_region(spritesheet, 780, 797, 20, 20, SCREEN_W-35, SCREEN_H-35, 0) : al_draw_bitmap_region(spritesheet, 780, 773, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
    }
    else
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (pxSize*mapSizeY)+((al_get_display_width(display)-pxSize*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(font2, al_map_rgb(255,255,255), (al_get_display_width(display))-136, (al_get_display_height(display))-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(font2, al_map_rgb(255,255,255), (al_get_display_width(display))-132, (al_get_display_height(display))-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "JOGAR NOVAMENTE?");
        al_draw_text(font2, al_map_rgb(255,255,255), TITLE_x, 320, ALLEGRO_ALIGN_CENTER, "SIM (Y) / NAO (N)");

        (win)? al_draw_text(font2, al_map_rgb(0,255,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "VOCE VENCEU!") : al_draw_text(font2, al_map_rgb(255,0,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "GAME OVER");

        // Altera o icone do auto falante que representa o som (mutado ou desmutado).
        (mute)? al_draw_bitmap_region(spritesheet, 780, 797, 20, 20, (al_get_display_width(display))-35, (al_get_display_height(display))-35, 0) : al_draw_bitmap_region(spritesheet, 780, 773, 20, 20, (al_get_display_width(display))-35, (al_get_display_height(display))-35, 0);
    }

    (win)? al_draw_text(font1, al_map_rgb(255,241,0), (mapSizeX*pxSize/2)-250, mapSizeY*pxSize/2, 0, "FIM DE JOGO!") : al_draw_text(font1, al_map_rgb(255,0,0), (mapSizeX*pxSize/2)-250, mapSizeY*pxSize/2, 0, "GAME OVER");
}

void Map::drawReadyText(void)
{
    al_draw_text(font2, al_map_rgb(255,241,0), 9*pxSize-8, 8*pxSize+12, 0, "READY!");
}

void Map::drawEndText(void)
{
    al_draw_text(font1, al_map_rgb(255,241,0), mapSizeX*pxSize/2-250, mapSizeY*pxSize/2, 0, "FIM DE JOGO!");
}

int Map::getMapSizeX(void)
{
    return mapSizeX;
}

int Map::getMapSizeY(void)
{
    return mapSizeY;
}

int Map::getScore(void)
{
    return score;
}

int Map::getHighScore(void)
{
    return highscore;
}

int Map::getDotCount(void)
{
    return dotCount;
}

int Map::getPxSize(void)
{
    return pxSize;
}

void Map::setTileMap(int x, int y, int val)
{
    tileMap[x][y] = val;
}

void Map::setScore(int _score)
{
    score = _score;
}

void Map::setHighScore(void)
{
    if(score > highscore)
        highscore = score;
}

void Map::setDotCount(int _dotCount)
{
    dotCount += _dotCount;
}
