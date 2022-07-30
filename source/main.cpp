#include "map.h"
#include "pacman.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define WINDOW_W 1160
#define WINDOW_H 760

/***********************************************************************************************************************************************************/

/** FUNÇÃO QUE ALTERNA O JOGO ENTRE JANELA / FULLSCREEN (bom para telas com resolução de até 1366x768) */

void switchDisplayMode(ALLEGRO_DISPLAY* _display, bool& fullscreen)
{
    (!fullscreen)? fullscreen = true : fullscreen = false;

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

// INÍCIO DA FUNÇÃO MAIN
int main(void)
{
    // Constantes para nomes de arquivos:
    const char* window_title = "PAC-MAN ::::: by: Reinaldo G P Neto";
    const char* icon_img_filename = "./img/pacman-icon.png";
    const char* sprites_img_filename = "./img/sprites.png";
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
    ALLEGRO_SAMPLE *startgame_sound = NULL;
    ALLEGRO_SAMPLE *waka_sound = NULL;
    ALLEGRO_SAMPLE *siren_sound = NULL;
    ALLEGRO_SAMPLE *pill_sound = NULL;
    ALLEGRO_SAMPLE *victory_sound = NULL;
    ALLEGRO_SAMPLE *death_sound = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_SAMPLE_ID siren_sound_id;

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

    /*******************************************     DISPLAY CONFIG.    *************************************************/

    // Config. display
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);

    int SCREEN_W, SCREEN_H;
    bool fullscreen = false;

    // Aqui, SCREEN_W e SCREEN_H são as dimensões da tela do usuário, adquiridas automaticamente pelo Allegro.
    SCREEN_W = info.x2 - info.x1;
    SCREEN_H = info.y2 - info.y1;

    // O display é criado inicialmente em modo janela nas proporções definidas por WINDOW_W e WINDOW_H.
    (fullscreen)? al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_FULLSCREEN_WINDOW) : al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED);

    display = al_create_display(WINDOW_W, WINDOW_H);

    /******************************************* Set display WIDESCREEN *************************************************/

    /** ** CONFIGs. ADICIONAIS NECESSÁRIAS! deixar essa parte comentada ** **/

    /*     al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
     *     display = al_create_display(SCREEN_W, SCREEN_H);
     *
     *     //     Aqui, o display é redimensionado em widescreen de acordo com a resolução definida para o display...
     *     //     ... e a resolução da tela do usuário.
     *
     *     if(fullscreen)
     *     {
     *         float RED_W = SCREEN_W / (float) WINDOW_W;
     *         float RED_H = SCREEN_H / (float) WINDOW_H;
     *
     *         ALLEGRO_TRANSFORM AT;
     *
     *         al_identity_transform(&AT);
     *
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
    const float FPS = 15.0;
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

    bool mute = false, win = false, gameover = false, playing = false, done = false, endGame = false, fastMode = false, redraw = true;
    const int qtdChar = 5;
    int eat_sound = 0;

    enum characters {PACMAN, BLINKY, PINKY, INKY, CLYDE};

    Map game_map;

    /* Array de ponteiros do tipo "Character", onde cada um é dinamicamente alocado com um dos personagens do jogo.
        Usando o enum "characters" fica bem fácil identificar qual é qual.*/
    Character* Char[qtdChar];

    Char[PACMAN] = new Pacman;
    Char[BLINKY] = new Blinky;
    Char[PINKY] = new Pinky;
    Char[INKY] = new Inky;
    Char[CLYDE] = new Clyde;

    /***********************************************************************************************************************************************************/

    /** EXECUÇÃO DO JOGO: **/

    if(!game_map.loadFonts())
    {
        al_show_native_message_box(display, "Error", "Error", "Falha ao carregar fontes!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    while(!done)
    {
        if(!game_map.loadMap())
        {
            al_show_native_message_box(display, "Fatal Error", "Error", "Falha ao carregar o mapa do jogo!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            done = true;
        }

        /** Tela inicial **/

        game_map.drawStartMap(display, sprites, fullscreen, mute, SCREEN_W, SCREEN_H);
        al_flip_display();

        /** Standby **/

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
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
                {
                    redraw = false;
                    game_map.drawActiveMap(display, sprites, fullscreen, mute, SCREEN_W, SCREEN_H);
                    game_map.drawReadyText();
                    Char[PACMAN]->drawCharacter(sprites, game_map.getPxSize());
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
                    switchDisplayMode(display, fullscreen);
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
                game_map.drawStartMap(display, sprites, fullscreen, mute, SCREEN_W, SCREEN_H);
                al_flip_display();
                redraw = false;
            }

        } // end of "not playing" loop

        /** Jogando **/

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
                case ALLEGRO_KEY_RIGHT: case ALLEGRO_KEY_D:
                    Char[PACMAN]->setIntention(RIGHT);
                    break;
                case ALLEGRO_KEY_DOWN: case ALLEGRO_KEY_S:
                    Char[PACMAN]->setIntention(DOWN);
                    break;
                case ALLEGRO_KEY_LEFT: case ALLEGRO_KEY_A:
                    Char[PACMAN]->setIntention(LEFT);
                    break;
                case ALLEGRO_KEY_UP: case ALLEGRO_KEY_W:
                    Char[PACMAN]->setIntention(UP);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    redraw = playing = false;
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
                    switchDisplayMode(display, fullscreen);
                    redraw = true;
                    break;
                }
            }
            else if(event.type == ALLEGRO_EVENT_TIMER)
            {

                /** MOVIMENTAÇÃO DO PLAYER (PACMAN) */

                Char[PACMAN]->moveCharacter(NULL, game_map);

                eat_sound = Char[PACMAN]->eatAction(game_map);

                if(eat_sound == 1)
                {
                    if(!mute)
                        al_play_sample(waka_sound, 4.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                else if(eat_sound == 2)
                {
                    if(!mute)
                        al_play_sample(pill_sound, 3.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }

                /** MOVIMENTAÇÃO DO FANTASMA BLINKY */

                // Faz com que o Blinky alterne entre os modos de movimento "perseguição" e "aleatório" a cada 150 ticks do timer.
//                if(al_get_timer_count(timer)%150 == 0)
//                {
//                    if(!Char[BLINKY]->getChasingStat())
//                    {
//                        Char[BLINKY]->switchCharStats(true, false);
//                    }
//                    else if(!Char[BLINKY]->getRandomStat())
//                    {
//                        Char[BLINKY]->switchCharStats(false, true);
//                    }
//                }

                // Realiza movimentos no modo de perseguição
                if(Char[BLINKY]->getChasingStat())
                {
                    if(Char[BLINKY]->getActiveStat() && !Char[BLINKY]->getMovingStat())
                    {
                        Char[BLINKY]->moveCharacter(Char[PACMAN], game_map);
                    }
                }
                // Realiza movimentos no modo aleatório
//                else if(Char[BLINKY]->getRandomStat())
//                {
//                    if(Char[BLINKY]->getActiveStat())
//                    {
//                        if(!Char[BLINKY]->getMovingStat() && (!Char[BLINKY]->collision(Char[PINKY]) && !Char[BLINKY]->collision(Char[INKY]) && !Char[BLINKY]->collision(Char[CLYDE])))
//                        {
//                            Char[BLINKY]->Character::moveCharacter(NULL, game_map);
//                        }
//                        else if(Char[BLINKY]->collision(Char[PINKY]) || Char[BLINKY]->collision(Char[INKY]) || Char[BLINKY]->collision(Char[CLYDE]))
//                        {
//                            Char[BLINKY]->setIntention(rand()%4);
//                            Char[BLINKY]->Character::moveCharacter(NULL, game_map);
//                        }
//                    }
//                }

                /** MOVIMENTAÇÃO DO FANTASMA PINKY */

                // Faz com que o Pinky alterne entre os modos de movimento "perseguição" e "aleatório" a cada 150 ticks do timer (sempre o oposto de Blinky).
//                if(al_get_timer_count(timer)%150 == 0)
//                {
//                    if(!Char[PINKY]->getChasingStat())
//                    {
//                        Char[PINKY]->switchCharStats(true, false);
//                    }
//                    else if(!Char[PINKY]->getRandomStat())
//                    {
//                        Char[PINKY]->switchCharStats(false, true);
//                    }
//                }

                // Realiza movimentos no modo de perseguição
                if(Char[PINKY]->getChasingStat())
                {
                    if(Char[PINKY]->getActiveStat() && !Char[PINKY]->getMovingStat())
                    {
                        Char[PINKY]->moveCharacter(Char[PACMAN], game_map);
                    }
                }
                // Realiza movimentos no modo aleatório
//                else if(Char[PINKY]->getRandomStat())
//                {
//                    if(Char[PINKY]->getActiveStat())
//                    {
//                        if(!Char[PINKY]->getMovingStat() && (!Char[PINKY]->collision(Char[BLINKY]) && !Char[PINKY]->collision(Char[INKY]) && !Char[PINKY]->collision(Char[CLYDE])))
//                        {
//                            Char[PINKY]->Character::moveCharacter(NULL, game_map);
//                        }
//                        else if(Char[PINKY]->collision(Char[BLINKY]) || Char[PINKY]->collision(Char[INKY]) || Char[PINKY]->collision(Char[CLYDE]))
//                        {
//                            Char[PINKY]->setIntention(rand()%4);
//                            Char[PINKY]->Character::moveCharacter(NULL, game_map);
//                        }
//                    }
//                }

                /** MOVIMENTAÇÃO DO FANTASMA INKY */ // (sempre random)

                if(Char[INKY]->getActiveStat())
                {
                    if(!Char[INKY]->getMovingStat() && (!Char[INKY]->collision(Char[BLINKY]) && !Char[INKY]->collision(Char[PINKY]) && !Char[INKY]->collision(Char[CLYDE])))
                    {
                        Char[INKY]->moveCharacter(NULL, game_map);
                    }
                    else if(Char[INKY]->collision(Char[BLINKY]) || Char[INKY]->collision(Char[PINKY]) || Char[INKY]->collision(Char[CLYDE]))
                    {
                        Char[INKY]->setIntention(rand()%4);
                        Char[INKY]->moveCharacter(NULL, game_map);
                    }
                }

                /** MOVIMENTAÇÃO DO FANTASMA CLYDE */ // (sempre random)

                if(Char[CLYDE]->getActiveStat())
                {
                    if(!Char[CLYDE]->getMovingStat() && (!Char[CLYDE]->collision(Char[BLINKY]) && !Char[CLYDE]->collision(Char[PINKY]) && !Char[CLYDE]->collision(Char[INKY])))
                    {
                        Char[CLYDE]->moveCharacter(NULL, game_map);
                    }
                    else if(Char[CLYDE]->collision(Char[BLINKY]) || Char[CLYDE]->collision(Char[PINKY]) || Char[CLYDE]->collision(Char[INKY]))
                    {
                        Char[CLYDE]->setIntention(rand()%4);
                        Char[CLYDE]->moveCharacter(NULL, game_map);
                    }
                }

                redraw = true;

            } // end of event.type (timer)

            /** ATIVAÇÃO DOS FANTASMAS */

            // Ativa os fantasmas Pinky, Inky e Clyde após X "ticks" do timer.

            if(al_get_timer_count(timer) == 150)
            {
                if(!Char[PINKY]->getActiveStat())
                {
                    Char[PINKY]->setActiveStat(true);
                    Char[PINKY]->deployCharacter();
                }
            }
            else if(al_get_timer_count(timer) == 250)
            {
                if(!Char[INKY]->getActiveStat())
                {
                    Char[INKY]->setActiveStat(true);
                    Char[INKY]->deployCharacter();
                }
            }
            else if(al_get_timer_count(timer) == 350)
            {
                if(!Char[CLYDE]->getActiveStat())
                {
                    Char[CLYDE]->setActiveStat(true);
                    Char[CLYDE]->deployCharacter();
                }
            }

            // A dificuldade do jogo aumenta quando restam 30 dots no mapa, os fantasmas ficam mais rápidos (exceto o Clyde, ele é meio lerdinho).
            if(game_map.getDotCount() == 30 && !fastMode)
            {
                if(!mute)
                {
                    al_stop_sample(&siren_sound_id);
                    al_play_sample(siren_sound, 1.5, 0.0, 1.2, ALLEGRO_PLAYMODE_LOOP, &siren_sound_id);
                }

                Char[BLINKY]->setMoveSpeed(0.25);
                Char[PINKY]->setMoveSpeed(0.25);
                Char[INKY]->setMoveSpeed(0.25);

                fastMode = true;
            }

            // Condição para perder o jogo
            if(Char[PACMAN]->collision(Char[BLINKY]) || Char[PACMAN]->collision(Char[PINKY]) || Char[PACMAN]->collision(Char[INKY]) || Char[PACMAN]->collision(Char[CLYDE]))
            {
                al_stop_timer(timer);
                playing = false;
                endGame = true;
                gameover = true;
            }

            // Condição para vencer o jogo
            if(game_map.getDotCount() == 0)
            {
                al_stop_timer(timer);
                playing = false;
                win = true;
                endGame = true;
            }

            // Só redesenha o mapa quando ocorre alguma mudança (se redraw = true).
            if(redraw)
            {
                game_map.drawActiveMap(display, sprites, fullscreen, mute, SCREEN_W, SCREEN_H);

                for(int i=0; i<qtdChar; i++)
                {
                    Char[i]->drawCharacter(sprites, game_map.getPxSize());
                }

                al_flip_display();
                redraw = false;
            }

        } // end of "playing" loop

        if(win)
        {
            if(!mute)
            {
                al_stop_sample(&siren_sound_id);
                al_play_sample(victory_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }

            game_map.setHighScore();

            game_map.drawActiveMap(display, sprites, fullscreen, mute, SCREEN_W, SCREEN_H);
            game_map.drawEndText();
            al_flip_display();

            al_rest(5);

            game_map.drawEndMap(display, sprites, fullscreen, mute, win, SCREEN_W, SCREEN_H);
            al_flip_display();
        }

        if(gameover)
        {
            al_stop_sample(&siren_sound_id);
            al_rest(1);

            if(!mute)
                al_play_sample(death_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

            Char[PACMAN]->deathAnimation(display, sprites, game_map, fullscreen, mute, SCREEN_W, SCREEN_H);

            game_map.setHighScore();

            game_map.drawEndMap(display, sprites, fullscreen, mute, win, SCREEN_W, SCREEN_H);
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
                    game_map.setScore(0);

                    for(int i=0; i<qtdChar; i++)
                        Char[i]->resetCharacter();

                    al_set_timer_count(timer, 0);
                    win = gameover = endGame = fastMode = false;
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
                    switchDisplayMode(display, fullscreen);
                    redraw = true;
                    break;
                }

            }

            // Só redesenha o mapa quando ocorre alguma mudança (se redraw = true).
            if(redraw)
            {
                game_map.drawEndMap(display, sprites, fullscreen, mute, win, SCREEN_W, SCREEN_H);
                al_flip_display();
                redraw = false;
            }

        } // end of "endGame" loop;

    } // end of "not done" loop;

    /***********************************************************************************************************************************************************/
    // FIM DE JOGO

    // Desalocando a memória alocada para o array de ponteiros "Character" ...
    for(int i=0; i<qtdChar; i++)
        delete Char[i];

    al_destroy_sample(startgame_sound);
    al_destroy_sample(waka_sound);
    al_destroy_sample(siren_sound);
    al_destroy_sample(pill_sound);
    al_destroy_sample(victory_sound);
    al_destroy_sample(death_sound);
    al_destroy_bitmap(window_icon);
    al_destroy_bitmap(sprites);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    return 0;

}  // the end
