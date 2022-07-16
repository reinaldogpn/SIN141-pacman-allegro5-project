/***********************************************************************************************************************************************************/
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <stdlib.h>

#include "brick.h"
#include "pill.h"
#include "character.h"
#include "pacman.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

/***********************************************************************************************************************************************************/

#define WINDOW_W 1160
#define WINDOW_H 760

// VARIÁVEIS GLOBAIS
const float FPS = 15.0;

int SCREEN_W, SCREEN_H, PIXELSIZE, mapSizeX, mapSizeY;
int score = 0, highscore = 0, pills = 0;
int tileMap[100][100] = { 0 };
bool mute = false, fullscreen = false, win = false, gameover = false;
enum direction { RIGHT, DOWN, LEFT, UP };

//enum character { Pac = 10, Shadow = 11, Speedy = 12, Bashful = 13, Pokey = 14 };
//Pac = Pac-Man, Shadow = Blinky, Speedy = Pinky, Bashful = Inky, Pokey = Clyde

/***********************************************************************************************************************************************************/

// PROTÓTIPOS
bool loadMap(const char* _map_filename);
void drawStartMap(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2);
void drawActiveMap(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2);
void drawEndMap(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2);
bool mapCollision(Character &C, int dir);
void pacmanEat(Pacman &P, ALLEGRO_SAMPLE* _sample2, ALLEGRO_SAMPLE* _sample4);
void switchDisplayMode(ALLEGRO_DISPLAY* _display);
void drawCharacters(ALLEGRO_BITMAP* _sprites, Character &Pac, Character &B, Character &P, Character &I, Character &C);
void moveGhost(Character &Ghost);
bool pacmanCollision(Character &Pac, Character &Ghost);
void pacmanDeathAnimation(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2, Character &Pac);

/***********************************************************************************************************************************************************/

// INÍCIO DA FUNÇÃO MAIN
int main(void)
{
    // Constantes para nomes de arquivos:
    const char* window_title = "PAC-MAN Beta";
    const char* map_filename = "./doc/map.txt";
    const char* icon_img_filename = "./img/pacman-icon.png";
    const char* sprites_img_filename = "./img/sprites.png";
    const char* font1_filename = "./ttf/crackman.ttf";
    const char* font2_filename = "./ttf/arcadepi.ttf";
    const char* sample1_filename = "./sound/beginning.wav";
    const char* sample2_filename = "./sound/pill.wav";
    const char* sample3_filename = "./sound/phase.wav";
    const char* sample4_filename = "./sound/eatsuperpill.wav";
    const char* sample5_filename = "./sound/win.wav";
    const char* sample6_filename = "./sound/death.wav";

    /***********************************************************************************************************************************************************/

    // ELEMENTOS DO ALLEGRO
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_BITMAP *window_icon = NULL;
    ALLEGRO_BITMAP *sprites = NULL;
    ALLEGRO_FONT *font1 = NULL;
    ALLEGRO_FONT *font2 = NULL;
    ALLEGRO_SAMPLE *startgame_sound = NULL;
    ALLEGRO_SAMPLE *waka_sound = NULL;
    ALLEGRO_SAMPLE *siren_sound = NULL;
    ALLEGRO_SAMPLE *pill_sound = NULL;
    ALLEGRO_SAMPLE *victory_sound = NULL;
    ALLEGRO_SAMPLE *death_sound = NULL;

    // Init Allegro
    if(!al_init())
    {
        al_show_native_message_box(display, "Error", "Error", "Falha ao iniciar o Allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -2;
    }

    // Set IMG addon
    if(!al_init_image_addon())
    {
        al_show_native_message_box(display, "Error", "Error", "Falha ao iniciar o image addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -3;
    }

    // Set primitives
    if(!al_init_primitives_addon())
    {
        al_show_native_message_box(display, "Error", "Error", "Falha ao iniciar o primitives addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -3;
    }

    // Set KEYBOARD module
    if(!al_install_keyboard())
    {
        al_show_native_message_box(display, "KEYBOARD", "Error", "Falha ao instalar módulo de teclado!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -4;
    }

    // Set FONTS addons
    if(!al_init_font_addon())
    {
        al_show_native_message_box(display, "FONTS", "Error", "Falha ao instalar módulo de fontes!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -3;
    }

    if(!al_init_ttf_addon())
    {
        al_show_native_message_box(display, "TTF", "Error", "Falha ao instalar módulo ttf!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -3;
    }

    // Set AUDIO modules, addons
    if(!al_init_acodec_addon())
    {
        al_show_native_message_box(display, "ACODEC", "Error", "Falha ao instalar módulo acodec!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -3;
    }

    if(!al_install_audio())
    {
        al_show_native_message_box(display, "AUDIO", "Error", "Falha ao instalar módulo de audio!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -4;
    }

    if(!al_reserve_samples(6))
    {
        al_show_native_message_box(display, "RESERVE SAMPLES", "Error", "Falha ao reservar samples!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -5;
    }

    /***********************************************************************************************************************************************************/

    // DEFINIÇÕES DOS PONTEIROS ALLEGRO

    // Config. display

    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);

    // Aqui, SCREEN_W e SCREEN_H são as dimensões da tela do usuário, adquiridas automaticamente pelo Allegro.
    SCREEN_W = info.x2 - info.x1;
    SCREEN_H = info.y2 - info.y1;

    // O display é criado inicialmente em modo janela nas proporções definidas por WINDOW_W e WINDOW_H.
    (fullscreen)? al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_FULLSCREEN_WINDOW) : al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED);

    display = al_create_display(WINDOW_W, WINDOW_H);

    /******************************************* Set display WIDESCREEN *************************************************/

    /*     al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
     *     display = al_create_display(SCREEN_W, SCREEN_H);
     *
     * //     Aqui, o display é redimensionado em widescreen de acordo com a resolução definida para o display...
     * //     ... e a resolução da tela do usuário.
     *
     *     if(fullscreen)
     *     {
     *         float RED_W = SCREEN_W / (float) WINDOW_W;
     *         float RED_H = SCREEN_H / (float) WINDOW_H;
     *
     *         ALLEGRO_TRANSFORM AT;
     *
     *         al_identity_transform(&AT);
     *         al_scale_transform(&AT, RED_W, RED_H);
     *         al_use_transform(&AT);
     *     }
     */

    /******************************************************************************************************************/

    if(!display)
    {
        al_show_native_message_box(display, "\"DISPLAY\"", "Error", "Falha ao iniciar o display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_clear_to_color(al_map_rgb(0,0,0));
    al_set_window_title(display, window_title);
    al_set_window_position(display, 200, 200);

    // Set TIMER
    timer = al_create_timer(1.0 / FPS);

    if(!timer)
    {
        al_show_native_message_box(display, "TIMER", "Error", "Falha ao iniciar o timer!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Set EVENT QUEUE
    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        al_show_native_message_box(display, "EVENT QUEUE", "Error", "Falha ao iniciar a fila de eventos!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Set IMGs
    window_icon = al_load_bitmap(icon_img_filename);
    if(!window_icon)
    {
        al_show_native_message_box(display, "ICON", "Error", "Falha ao criar a imagem referenciada!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }
    al_set_display_icon(display, window_icon);

    sprites = al_load_bitmap(sprites_img_filename);
    if(!sprites)
    {
        al_show_native_message_box(display, "SPRITESHEET", "Error", "Falha ao criar a imagem referenciada!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }

    // Set FONTs
    font1 = al_load_font(font1_filename, 60, 0);
    if(!font1)
    {
        al_show_native_message_box(display, "FONT1", "Error", "Falha ao iniciar fonte!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    font2 = al_load_font(font2_filename, 14, 0);
    if(!font2)
    {
        al_show_native_message_box(display, "FONT2", "Error", "Falha ao iniciar fonte!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Set AUDIOs
    startgame_sound = al_load_sample(sample1_filename);
    if (!startgame_sound)
    {
        al_show_native_message_box(display, "SAMPLE1", "Error", "Falha ao iniciar sample 1!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    waka_sound = al_load_sample(sample2_filename);
    if (!waka_sound)
    {
        al_show_native_message_box(display, "SAMPLE2", "Error", "Falha ao iniciar sample 2!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    siren_sound = al_load_sample(sample3_filename);
    if (!siren_sound)
    {
        al_show_native_message_box(display, "SAMPLE3", "Error", "Falha ao iniciar sample 3!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    pill_sound = al_load_sample(sample4_filename);
    if (!pill_sound)
    {
        al_show_native_message_box(display, "SAMPLE4", "Error", "Falha ao iniciar sample 4!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    victory_sound = al_load_sample(sample5_filename);
    if (!victory_sound)
    {
        al_show_native_message_box(display, "SAMPLE5", "Error", "Falha ao iniciar sample 5!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    death_sound = al_load_sample(sample6_filename);
    if (!death_sound)
    {
        al_show_native_message_box(display, "SAMPLE5", "Error", "Falha ao iniciar sample 6!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    /***********************************************************************************************************************************************************/

    // Classes:
    Brick B;
    PIXELSIZE = B.getBH();
    Pill P;

    Pacman pacman;
    Blinky blinky;
    Pinky pinky;
    Inky inky;
    Clyde clyde;

    // Outras variáveis locais:
    bool playing = false, redraw = true, done = false, endGame = false, fastMode = false;

    srand(time(nullptr));

    ALLEGRO_EVENT event;
    ALLEGRO_SAMPLE_ID siren_sound_id;

    /***********************************************************************************************************************************************************/
    // Execução do jogo:

    while(!done)
    {
        // TELA INICIAL
        if(!loadMap(map_filename))
        {
            done = true;
            break;
        }

        drawStartMap(B, P, display, sprites, font1, font2);
        al_flip_display();

        // STANDBY
        while(!playing) // Enquanto não estiver jogando (standby)...
        {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
                al_acknowledge_resize(display);
                redraw = true;
            }
            else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    redraw = false;
                    drawActiveMap(B, P, display, sprites, font1, font2);
                    al_draw_text(font2, al_map_rgb(255,241,0), 9*PIXELSIZE-8, 8*PIXELSIZE+12, 0, "READY!");
                    pacman.drawCharacter(sprites, PIXELSIZE);
                    al_flip_display();

                    if(!mute)
                        al_play_sample(startgame_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                    al_rest(4.5);
                    playing = true;
                }
                else if(event.keyboard.keycode == ALLEGRO_KEY_M)
                {
                    (!mute)? mute = true: mute = false;
                    redraw = true;
                }
                else if(event.keyboard.keycode == ALLEGRO_KEY_F)
                {
                    switchDisplayMode(display);
                    redraw = true;
                }
                else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    done = true;
                    break;
                }
            }
            else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                done = true;
                break;
            }

            if(redraw)
            {
                drawStartMap(B, P, display, sprites, font1, font2);
                al_flip_display();
                redraw = false;
            }

        } // end of "not playing" loop

        // JOGANDO
        if(!mute)
            al_play_sample(siren_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &siren_sound_id);

        al_start_timer(timer);

        while(playing)
        {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
                al_acknowledge_resize(display);
                redraw = true;
            }
            else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                playing = false;
                done = true;
            }
            else if(event.type == ALLEGRO_EVENT_KEY_DOWN) // Quando o jogador pressiona uma das teclas de movimento, é definida a "intenção" de movimento do pacman.
            {
                switch(event.keyboard.keycode)
                {
                case ALLEGRO_KEY_RIGHT:
                    pacman.setIntention(RIGHT);
                    break;
                case ALLEGRO_KEY_DOWN:
                    pacman.setIntention(DOWN);
                    break;
                case ALLEGRO_KEY_LEFT:
                    pacman.setIntention(LEFT);
                    break;
                case ALLEGRO_KEY_UP:
                    pacman.setIntention(UP);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    redraw = false;
                    playing = false;
                    done = true;
                    break;
                case ALLEGRO_KEY_M:
                    (!mute)? mute = true: mute = false;

                    if(mute)
                        al_stop_sample(&siren_sound_id);
                    else if(fastMode)
                        al_play_sample(siren_sound, 1.5, 0.0, 1.2, ALLEGRO_PLAYMODE_LOOP, &siren_sound_id);
                    else
                        al_play_sample(siren_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &siren_sound_id);

                    redraw = true;
                    break;
                case ALLEGRO_KEY_F:
                    switchDisplayMode(display);
                    redraw = true;
                    break;
                }
            }
            else if(event.type == ALLEGRO_EVENT_TIMER)
            {
                switch(pacman.getIntention())                                       /** Aqui acontece a movimentação do player de acordo com sua "intenção" (Player.getPlayerIntention()) de movimento... */
                {                                                                   /** ... se não for possível se mover na direção pretendida, o pacman continua se movendo para sua "última direção" (lastDir); */
                case RIGHT:                                                         /** A última direção (lastDir) do pacman é definida quando ele se move com sucesso para qualquer direção. */
                    if(!mapCollision(pacman, RIGHT))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(RIGHT);                                  /** A função "mapCollision()" testa se é possível se mover na direção pretendida; */
                        pacman.setLastDir(RIGHT);                                   /** A função retorna (true) caso a próxima pos. do pacman corresponda a uma parede (brick) ou a uma "ghostzone" (área de spawn dos fantasmas)... */
                    }                                                               /** ... ou caso o pacman encontre uma das paredes externas do labirinto, nesses casos, há uma colisão... */
                    else if(!mapCollision(pacman, pacman.getLastDir()))             /** ... caso o contrário, a função retorna (false) e assim, o pacman pode se mover. */
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(pacman.getLastDir());
                    }
                    else
                        pacman.isMoving(false);

                    // Config. do "túnel" dir.: se o player chegar na pos. (18, 8) e andar para a direita, ele é teleportado para a pos. (0, 8)
                    if(pacman.getPX() == mapSizeX-1 && pacman.getPY() == 8)
                        pacman.setPX(0);

                    redraw = true;
                    break;
                case DOWN:
                    if(!mapCollision(pacman, DOWN))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(DOWN);
                        pacman.setLastDir(DOWN);
                    }
                    else if(!mapCollision(pacman, pacman.getLastDir()))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(pacman.getLastDir());
                    }
                    else
                        pacman.isMoving(false);

                    redraw = true;
                    break;
                case LEFT:
                    if(!mapCollision(pacman, LEFT))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(LEFT);
                        pacman.setLastDir(LEFT);
                    }
                    else if(!mapCollision(pacman, pacman.getLastDir()))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(pacman.getLastDir());
                    }
                    else
                        pacman.isMoving(false);

                    // Config. do "túnel" esq.: se o player chegar na pos. (0, 8) e andar para a esquerda, ele é teleportado para a pos. (18, 8)
                    if(pacman.getPX() == 0 && pacman.getPY() == 8)
                        pacman.setPX(mapSizeX-1);

                    redraw = true;
                    break;
                case UP:
                    if(!mapCollision(pacman, UP))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(UP);
                        pacman.setLastDir(UP);
                    }
                    else if(!mapCollision(pacman, pacman.getLastDir()))
                    {
                        pacman.isMoving(true);
                        pacman.setMovement(pacman.getLastDir());
                    }
                    else
                        pacman.isMoving(false);

                    redraw = true;
                    break;
                }

                moveGhost(blinky);
                moveGhost(pinky);
                moveGhost(inky);
                moveGhost(clyde);

                // Condição para vencer o jogo
                if(pills == 0)
                {
                    al_stop_timer(timer);
                    playing = false;
                    win = true;
                    endGame = true;
                }

                // Ativa os fantasmas Pinky, Inky e Clyde após X "ticks" do timer.
                if(al_get_timer_count(timer) == 100)
                {
                    if(!pinky.isActive())
                    {
                        pinky.setActive(true);
                        pinky.setPX(9);
                        pinky.setPY(6);
                    }
                }
                else if(al_get_timer_count(timer) == 200)
                {
                    if(!inky.isActive())
                    {
                        inky.setActive(true);
                        inky.setPX(9);
                        inky.setPY(6);
                    }
                }
                else if(al_get_timer_count(timer) == 300)
                {
                    if(!clyde.isActive())
                    {
                        clyde.setActive(true);
                        clyde.setPX(9);
                        clyde.setPY(6);
                    }
                }

            }

            // Só redesenha o mapa quando ocorre alguma mudança (se redraw = true).
            if(redraw)
            {
                pacmanEat(pacman, waka_sound, pill_sound);
                drawActiveMap(B, P, display, sprites, font1, font2);
                drawCharacters(sprites, pacman, blinky, pinky, inky, clyde);
                al_flip_display();
                redraw = false;
            }

            // Condição para perder o jogo
            if(pacmanCollision(pacman, blinky) || pacmanCollision(pacman, pinky) || pacmanCollision(pacman, inky) || pacmanCollision(pacman, clyde))
            {
                al_stop_timer(timer);
                playing = false;
                endGame = true;
                gameover = true;
            }

            if(pills == 80 && !fastMode) // A dificuldade do jogo aumenta ao coletar metade das pílulas, os fantasmas ficam mais rápidos (exceto o Clyde, ele é meio lerdinho).
            {
                if(!mute)
                {
                    al_stop_sample(&siren_sound_id);
                    al_play_sample(siren_sound, 1.5, 0.0, 1.2, ALLEGRO_PLAYMODE_LOOP, &siren_sound_id);
                }

                blinky.setMoveSpeed(0.55);
                pinky.setMoveSpeed(0.45);
                inky.setMoveSpeed(0.35);

                fastMode = true;

                cout << blinky.getCharName() << " diz: Estou ficando p***!" << endl;
                cout << pinky.getCharName() << " diz: Estou também!" << endl;
                cout << inky.getCharName() << " diz: Eu também! ... Um pouco!" << endl;
                cout << clyde.getCharName() << " diz: ... zzZzzZzz ..." << endl;
            }

        } // end of "playing" loop

        if(win)
        {
            if(!mute)
            {
                al_stop_sample(&siren_sound_id);
                al_play_sample(victory_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }

            if(score > highscore)
                highscore = score;

            drawActiveMap(B, P, display, sprites, font1, font2);
            al_draw_text(font1, al_map_rgb(255,241,0), mapSizeX*PIXELSIZE/2-250, mapSizeY*PIXELSIZE/2, 0, "FIM DE JOGO!");
            al_flip_display();

            al_rest(5);

            drawEndMap(B, P, display, sprites, font1, font2);
            al_flip_display();
        }

        if(gameover)
        {
            al_stop_sample(&siren_sound_id);
            al_rest(1);

            if(!mute)
                al_play_sample(death_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

            pacmanDeathAnimation(B, P, display, sprites, font1, font2, pacman);

            if(score > highscore)
                highscore = score;

            drawEndMap(B, P, display, sprites, font1, font2);
            al_flip_display();
        }

        while(endGame)
        {
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
                al_acknowledge_resize(display);
                redraw = true;
            }
            else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                endGame = false;
                done = true;
            }
            else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(event.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    endGame = false;
                    done = true;
                    break;
                case ALLEGRO_KEY_Y:
                    score = 0;
                    pacman.resetPacman();
                    blinky.resetGhost();
                    pinky.resetGhost();
                    inky.resetGhost();
                    clyde.resetGhost();
                    al_set_timer_count(timer, 0);
                    win = false;
                    gameover = false;
                    endGame = false;
                    fastMode = false;
                    break;
                case ALLEGRO_KEY_N:
                    endGame = false;
                    done = true;
                    break;
                case ALLEGRO_KEY_M:
                    (!mute)? mute = true: mute = false;
                    redraw = true;
                    break;
                case ALLEGRO_KEY_F:
                    switchDisplayMode(display);
                    redraw = true;
                    break;
                }

            }

            // Só redesenha o mapa quando ocorre alguma mudança (se redraw = true).
            if(redraw)
            {
                drawEndMap(B, P, display, sprites, font1, font2);
                al_flip_display();
                redraw = false;
            }

        } // end of "endGame" loop;

    } // end of "not done" loop;

    /***********************************************************************************************************************************************************/
    // FIM DE JOGO

    al_destroy_sample(startgame_sound);
    al_destroy_sample(waka_sound);
    al_destroy_sample(siren_sound);
    al_destroy_sample(pill_sound);
    al_destroy_sample(victory_sound);
    al_destroy_sample(death_sound);
    al_destroy_font(font2);
    al_destroy_font(font1);
    al_destroy_bitmap(window_icon);
    al_destroy_bitmap(sprites);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    return 0;

}  // end of "main"

/***********************************************************************************************************************************************************/

// FUNÇãO QUE CARREGA O MAPA

bool loadMap(const char* _map_filename)
{
    int contX = 0, contY = 0;
    pills = 0;

    ifstream openfile(_map_filename);                                           /** Abre o arquivo .txt no qual está contida a matriz usada para criar o mapa do jogo.*/
    if(openfile.is_open())                                                      /** Se o arquivo for aberto com êxito... */
    {
        openfile >> mapSizeX >> mapSizeY;                                       /** Define as dimensões da matriz do mapa de acordo com os valores descritos na primeira linha do arquivo .txt */

        while(!openfile.eof())                                                  /** Enquanto não chegar no fim do arquivo... */
        {
            openfile >> tileMap[contX][contY];                                  /** Lê os valores da matriz de cima pra baixo, um por um e insere os valores nas respectivas posições da matriz... */

            if(tileMap[contX][contY] == 2 || tileMap[contX][contY] == 3)        /** Lê o número de pílulas que vão haver no mapa e incrementa o contador de pílulas. */
                pills++;

            contX++;

            if(contX >= mapSizeX)                                               /** ... quando o contador de linhas (contX) chegar na última linha do arquivo, ele volta para a linha 0 e o ... */
            {                                                                   /** ... contador de colunas (contY) incrementa +1... */
                contX = 0;
                contY++;
            }                                                                   /** ... e assim o processo é repetido até 'EOF' e a matriz ser completamente preenchida. */
        }                                                                       /** Esse método me permite alterar o mapa posteriormente sem a necessidade de recompilar todo o jogo. */
    }
    else
    {
        al_show_native_message_box(NULL, "MAP.TXT", "Error", "Falha ao abrir o arquivo referenciado!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    openfile.close();

    return true;
}

/***********************************************************************************************************************************************************/

// FUNÇãO QUE DESENHA O MAPA INICIAL

void drawStartMap(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2)
{
    // Criei essas variaveis para as funçoes de desenhar não ficarem tão poluídas...
    float PX = _P.getPX();
    float PY = _P.getPY();
    float PW = _P.getPW();
    float PH = _P.getPH();
    float BX = _B.getBX();
    float BY = _B.getBY();
    float BW = _B.getBW();
    float BH = _B.getBH();

    al_clear_to_color(al_map_rgb(0,0,0));

    for(int i=0; i<mapSizeX; i++)
    {
        for(int j=0; j<mapSizeY; j++)
        {
            if(tileMap[i][j] == 1) // Valor na matriz igual a 1 = Tijolo;
            {
                al_draw_bitmap_region(_sprites, BX, BY, BH, BW, i*PIXELSIZE, j*PIXELSIZE, 0);
            }
            else if(tileMap[i][j] == 2) // Valor na matriz igual a 2 = Pílula;
            {
                // Aqui são feitos alguns ajustes para a pílula aparecer exatamente no centro de cada célula do mapa, tal que BX, BY = 40. A expressão fica assim: (i*40)+(20)-1 // (j*40)+(20)-1, sendo i e j as posições na matriz do mapa.
                al_draw_bitmap_region(_sprites, PX, PY, PW, PH, ((i*PIXELSIZE)+(PIXELSIZE/2)-1), ((j*PIXELSIZE)+(PIXELSIZE/2)-1), 0);
            }
            else if(tileMap[i][j] == 3) // Valor na matriz igual a 3 = Super pílula;
            {
                al_draw_bitmap_region(_sprites, 411, 313, 16, 16, ((i*PIXELSIZE)+(PIXELSIZE/2)-7), ((j*PIXELSIZE)+(PIXELSIZE/2)-7), 0);
            }

        }

    }

    // Portinha da ghostzone
    al_draw_filled_rectangle(360.5, 7*PIXELSIZE+0.5, 360+PIXELSIZE, 7*PIXELSIZE+6, al_map_rgb(0,18,173));

    // Funçoes que desenham o texto permanente da tela do jogo.
    if(fullscreen)
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (PIXELSIZE*mapSizeY)+((SCREEN_W-PIXELSIZE*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(_font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(_font2, al_map_rgb(255,255,255), SCREEN_W-136, SCREEN_H-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(_font2, al_map_rgb(255,255,255), SCREEN_W-132, SCREEN_H-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(_font2, al_map_rgb(255,241,0), 9*PIXELSIZE-8, 8*PIXELSIZE+12, 0, "READY!");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "PRESSIONE ENTER PARA INICIAR!");

        if(mute) // Altera o icone do auto falante que representa o som (mutado ou desmutado).
            al_draw_bitmap_region(_sprites, 780, 797, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
        else
            al_draw_bitmap_region(_sprites, 780, 773, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
    }
    else
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (PIXELSIZE*mapSizeY)+((al_get_display_width(_display)-PIXELSIZE*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(_font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(_font2, al_map_rgb(255,255,255), (al_get_display_width(_display))-136, (al_get_display_height(_display))-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(_font2, al_map_rgb(255,255,255), (al_get_display_width(_display))-132, (al_get_display_height(_display))-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(_font2, al_map_rgb(255,241,0), 9*PIXELSIZE-8, 8*PIXELSIZE+12, 0, "READY!");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "PRESSIONE ENTER PARA INICIAR!");

        if(mute) // Altera o icone do auto falante que representa o som (mutado ou desmutado).
            al_draw_bitmap_region(_sprites, 780, 797, 20, 20, (al_get_display_width(_display))-35, (al_get_display_height(_display))-35, 0);
        else
            al_draw_bitmap_region(_sprites, 780, 773, 20, 20, (al_get_display_width(_display))-35, (al_get_display_height(_display))-35, 0);
    }
}

/***********************************************************************************************************************************************************/

// FUNÇãO QUE DESENHA O MAPA DO JOGO ATIVO

void drawActiveMap(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2)
{
    // Criei essas variaveis para as funçoes de desenhar não ficarem tão poluídas...
    float PX = _P.getPX();
    float PY = _P.getPY();
    float PW = _P.getPW();
    float PH = _P.getPH();
    float BX = _B.getBX();
    float BY = _B.getBY();
    float BW = _B.getBW();
    float BH = _B.getBH();

    al_clear_to_color(al_map_rgb(0,0,0));

    for(int i=0; i<mapSizeX; i++)
    {
        for(int j=0; j<mapSizeY; j++)
        {
            if(tileMap[i][j] == 1) // Valor na matriz igual a 1 = Tijolo;
            {
                al_draw_bitmap_region(_sprites, BX, BY, BH, BW, i*PIXELSIZE, j*PIXELSIZE, 0);
            }
            else if(tileMap[i][j] == 2) // Valor na matriz igual a 2 = Pílula;
            {
                // Aqui são feitos alguns ajustes para a pílula aparecer exatamente no centro de cada célula do mapa, tal que BX, BY = 40. A expressão fica assim: (i*40)+(20)-1 // (j*40)+(20)-1, sendo i e j as posições na matriz do mapa.
                al_draw_bitmap_region(_sprites, PX, PY, PW, PH, ((i*PIXELSIZE)+(PIXELSIZE/2)-1), ((j*PIXELSIZE)+(PIXELSIZE/2)-1), 0);
            }
            else if(tileMap[i][j] == 3) // Valor na matriz igual a 3 = Super pílula;
            {
                al_draw_bitmap_region(_sprites, 411, 313, 16, 16, ((i*PIXELSIZE)+(PIXELSIZE/2)-7), ((j*PIXELSIZE)+(PIXELSIZE/2)-7), 0);
            }

        }

    }

    // Portinha da ghostzone
    al_draw_filled_rectangle(360.5, 7*PIXELSIZE+0.5, 360+PIXELSIZE, 7*PIXELSIZE+6, al_map_rgb(0,18,173));

    // Funçoes que desenham o texto permanente da tela do jogo.
    if(fullscreen)
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (PIXELSIZE*mapSizeY)+((SCREEN_W-PIXELSIZE*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(_font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(_font2, al_map_rgb(255,255,255), SCREEN_W-136, SCREEN_H-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(_font2, al_map_rgb(255,255,255), SCREEN_W-132, SCREEN_H-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");

        if(mute) // Altera o icone do auto falante que representa o som (mutado ou desmutado).
            al_draw_bitmap_region(_sprites, 780, 797, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
        else
            al_draw_bitmap_region(_sprites, 780, 773, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
    }
    else
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (PIXELSIZE*mapSizeY)+((al_get_display_width(_display)-PIXELSIZE*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(_font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(_font2, al_map_rgb(255,255,255), (al_get_display_width(_display))-136, (al_get_display_height(_display))-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(_font2, al_map_rgb(255,255,255), (al_get_display_width(_display))-132, (al_get_display_height(_display))-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");

        if(mute) // Altera o icone do auto falante que representa o som (mutado ou desmutado).
            al_draw_bitmap_region(_sprites, 780, 797, 20, 20, (al_get_display_width(_display))-35, (al_get_display_height(_display))-35, 0);
        else
            al_draw_bitmap_region(_sprites, 780, 773, 20, 20, (al_get_display_width(_display))-35, (al_get_display_height(_display))-35, 0);
    }
}

/***********************************************************************************************************************************************************/

// FUNÇãO QUE DESENHA O MAPA DE FIM DE JOGO

void drawEndMap(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2)
{
    // Criei essas variaveis para as funçoes de desenhar não ficarem tão poluídas...
    float PX = _P.getPX();
    float PY = _P.getPY();
    float PW = _P.getPW();
    float PH = _P.getPH();
    float BX = _B.getBX();
    float BY = _B.getBY();
    float BW = _B.getBW();
    float BH = _B.getBH();

    al_clear_to_color(al_map_rgb(0,0,0));

    for(int i=0; i<mapSizeX; i++)
    {
        for(int j=0; j<mapSizeY; j++)
        {
            if(tileMap[i][j] == 1) // Valor na matriz igual a 1 = Tijolo;
            {
                al_draw_bitmap_region(_sprites, BX, BY, BH, BW, i*PIXELSIZE, j*PIXELSIZE, 0);
            }
            else if(tileMap[i][j] == 2) // Valor na matriz igual a 2 = Pílula;
            {
                // Aqui são feitos alguns ajustes para a pílula aparecer exatamente no centro de cada célula do mapa, tal que BX, BY = 40. A expressão fica assim: (i*40)+(20)-1 // (j*40)+(20)-1, sendo i e j as posições na matriz do mapa.
                al_draw_bitmap_region(_sprites, PX, PY, PW, PH, ((i*PIXELSIZE)+(PIXELSIZE/2)-1), ((j*PIXELSIZE)+(PIXELSIZE/2)-1), 0);
            }
            else if(tileMap[i][j] == 3) // Valor na matriz igual a 3 = Super pílula;
            {
                al_draw_bitmap_region(_sprites, 411, 313, 16, 16, ((i*PIXELSIZE)+(PIXELSIZE/2)-7), ((j*PIXELSIZE)+(PIXELSIZE/2)-7), 0);
            }
        }
    }

    // Portinha da ghostzone
    al_draw_filled_rectangle(360.5, 7*PIXELSIZE+0.5, 360+PIXELSIZE, 7*PIXELSIZE+6, al_map_rgb(0,18,173));

    // Funçoes que desenham o texto permanente da tela do jogo.
    if(fullscreen)
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (PIXELSIZE*mapSizeY)+((SCREEN_W-PIXELSIZE*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(_font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(_font2, al_map_rgb(255,255,255), SCREEN_W-136, SCREEN_H-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(_font2, al_map_rgb(255,255,255), SCREEN_W-132, SCREEN_H-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "JOGAR NOVAMENTE?");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x, 320, ALLEGRO_ALIGN_CENTER, "SIM (Y) / NAO (N)");

        (win)? al_draw_text(_font2, al_map_rgb(0,255,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "VOCE VENCEU!") : al_draw_text(_font2, al_map_rgb(255,0,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "GAME OVER");

        if(mute) // Altera o icone do auto falante que representa o som (mutado ou desmutado).
            al_draw_bitmap_region(_sprites, 780, 797, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
        else
            al_draw_bitmap_region(_sprites, 780, 773, 20, 20, SCREEN_W-35, SCREEN_H-35, 0);
    }
    else
    {
        // Config. para o texto (título do jogo e placar) ser desenhado no centro do painel à direita do mapa do jogo.
        int TITLE_x = (PIXELSIZE*mapSizeY)+((al_get_display_width(_display)-PIXELSIZE*mapSizeY)/2);
        int TITLE_y = 50;

        al_draw_text(_font1, al_map_rgb(255,241,0), TITLE_x, TITLE_y, ALLEGRO_ALIGN_CENTER, "PAC-MAN");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x-150, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x-150, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", score);
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x+5, TITLE_y+100, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        al_draw_textf(_font2, al_map_rgb(255, 255, 255), TITLE_x+5, TITLE_y+125, ALLEGRO_ALIGN_LEFT, "%d", highscore);
        al_draw_text(_font2, al_map_rgb(255,255,255), (al_get_display_width(_display))-136, (al_get_display_height(_display))-52, ALLEGRO_ALIGN_CENTER, "F = FULLSCREEN");
        al_draw_text(_font2, al_map_rgb(255,255,255), (al_get_display_width(_display))-132, (al_get_display_height(_display))-32, ALLEGRO_ALIGN_CENTER, "M = MUTE ON/OFF");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x, 300, ALLEGRO_ALIGN_CENTER, "JOGAR NOVAMENTE?");
        al_draw_text(_font2, al_map_rgb(255,255,255), TITLE_x, 320, ALLEGRO_ALIGN_CENTER, "SIM (Y) / NAO (N)");

        (win)? al_draw_text(_font2, al_map_rgb(0,255,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "VOCE VENCEU!") : al_draw_text(_font2, al_map_rgb(255,0,0), TITLE_x, 280, ALLEGRO_ALIGN_CENTER, "GAME OVER");

        if(mute) // Altera o icone do auto falante que representa o som (mutado ou desmutado).
            al_draw_bitmap_region(_sprites, 780, 797, 20, 20, (al_get_display_width(_display))-35, (al_get_display_height(_display))-35, 0);
        else
            al_draw_bitmap_region(_sprites, 780, 773, 20, 20, (al_get_display_width(_display))-35, (al_get_display_height(_display))-35, 0);
    }

    (win)? al_draw_text(_font1, al_map_rgb(255,241,0), (mapSizeX*PIXELSIZE/2)-250, mapSizeY*PIXELSIZE/2, 0, "FIM DE JOGO!") : al_draw_text(_font1, al_map_rgb(255,0,0), (mapSizeX*PIXELSIZE/2)-250, mapSizeY*PIXELSIZE/2, 0, "GAME OVER");
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE CHECA SE Há COLISÃO DO PACMAN COM O MAPA

bool mapCollision(Character &C, int dir)
{
    switch(dir) // Aqui o programa checa se a próxima posição do personagem é uma parede ou "ghostzone"; se sim, há colisão (retorna true); se não, não há colisão (retorna false).
    {
    case RIGHT: // RIGHT
        if(tileMap[C.getPX()+(1)][C.getPY()] != 1 && tileMap[C.getPX()+(1)][C.getPY()] != 4 && C.getPX()+1 <= mapSizeX-1)
        {
            return false;
        }
        break;
    case DOWN: // DOWN
        if(tileMap[C.getPX()][C.getPY()+(1)] != 1 && tileMap[C.getPX()][C.getPY()+(1)] != 4 && C.getPY()+1 < mapSizeY-1)
        {
            return false;
        }
        break;
    case LEFT: // LEFT
        if(tileMap[C.getPX()-(1)][C.getPY()] != 1 && tileMap[C.getPX()-(1)][C.getPY()] != 4 && C.getPX()-1 >= 0)
        {
            return false;
        }
        break;
    case UP: // UP
        if(tileMap[C.getPX()][C.getPY()-(1)] != 1 && tileMap[C.getPX()][C.getPY()-(1)] != 4 && C.getPY()-1 > 0)
        {
            return false;
        }
        break;
    }

    return true;
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE REPRODUZ A AÇÃO DE "COMER" DO PACMAN

void pacmanEat(Pacman &P, ALLEGRO_SAMPLE* _sample2, ALLEGRO_SAMPLE* _sample4)
{
    // Se o player "comer" uma pílula ou super pílula... esvazia a célula, incrementa o placar e decrementa o contador de pílulas.
    if(tileMap[P.getPX()][P.getPY()] == 2)
    {
        if(!mute)
            al_play_sample(_sample2, 4.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

        tileMap[P.getPX()][P.getPY()] = 0;
        score++;
        pills--;
    }
    else if(tileMap[P.getPX()][P.getPY()] == 3)
    {
        if(!mute)
            al_play_sample(_sample4, 3.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

        tileMap[P.getPX()][P.getPY()] = 0;
        score += 100;
        pills--;
    }
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE ALTERNA O JOGO ENTRE JANELA / FULLSCREEN

void switchDisplayMode(ALLEGRO_DISPLAY* _display)
{
    (!fullscreen)? fullscreen = true: fullscreen = false;

    if(fullscreen)
    {
        al_set_display_flag(_display, ALLEGRO_RESIZABLE, false);
        al_set_display_flag(_display, ALLEGRO_FULLSCREEN_WINDOW, true);
    }
    else
    {
        al_set_display_flag(_display, ALLEGRO_FULLSCREEN_WINDOW, false);
        al_set_display_flag(_display, ALLEGRO_RESIZABLE, true);
    }
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE DESENHA TODOS OS PERSONAGENS NA TELA

void drawCharacters(ALLEGRO_BITMAP* _sprites, Character &Pac, Character &B, Character &P, Character &I, Character &C)
{
    Pac.drawCharacter(_sprites, PIXELSIZE);
    B.drawCharacter(_sprites, PIXELSIZE);
    P.drawCharacter(_sprites, PIXELSIZE);
    I.drawCharacter(_sprites, PIXELSIZE);
    C.drawCharacter(_sprites, PIXELSIZE);
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE REALIZA A MOVIMENTAÇÃO ALEATÓRIA DOS FANTASMAS

void moveGhost(Character &Ghost)
{


    if(!mapCollision(Ghost, Ghost.getIntention()) && Ghost.isActive())
    {
        Ghost.isMoving(true);
        Ghost.setMovement(Ghost.getIntention());
        Ghost.setLastDir(Ghost.getIntention());
    }
    else
    {
        Ghost.isMoving(false);
        Ghost.setIntention(rand()%4);
    }
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE CHECA SE HÁ COLISÃO ENTRE O PACMAN E OS FANTASMAS

bool pacmanCollision(Character &Pac, Character &Ghost)
{
    int PacX = Pac.getPX();
    int PacY = Pac.getPY();
    int GhostX = Ghost.getPX();
    int GhostY = Ghost.getPY();

    if(PacX != GhostX || PacY != GhostY)
        return false;
    else
        return true;
}

/***********************************************************************************************************************************************************/

// FUNÇÃO QUE REALIZA A ANIMAÇÃO DO PACMAN MORRENDO

void pacmanDeathAnimation(Brick &_B, Pill &_P, ALLEGRO_DISPLAY* _display, ALLEGRO_BITMAP* _sprites, ALLEGRO_FONT* _font1, ALLEGRO_FONT* _font2, Character &Pac)
{
    float x = 351;
    float y = 8;
    float w = 37;
    float h = 40;
    float px = Pac.getPX();
    float py = Pac.getPY();

    // 11 sprites são usadas para essa animação.
    for(int i=0; i<11; i++)
    {
        drawActiveMap(_B, _P, _display, _sprites, _font1, _font2);
        al_draw_bitmap_region(_sprites, x, y, w, h, (int)px*PIXELSIZE+4, (int)py*PIXELSIZE+3, 0);
        al_flip_display();
        al_rest(0.15);
        y+=50;
    }
}
