#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "run.h"
#include "structs.h"
#include "draw.h"
#include "update.h"

/* ------------- Funcoes internas ------------- */

void drawMainMenu(gameManager_t *gm)
{
    al_draw_bitmap(gm->backgrounds[START_SCREEN], 0, 0, 0);
}

void drawTopScoreUI(gameManager_t *gm)
{
    al_draw_bitmap(gm->otherBitmaps[TOP_SCORE_UI], 250, 0, 0);
}

void drawCreditsUI(gameManager_t *gm)
{
    al_draw_bitmap(gm->otherBitmaps[CREDITS_UI], 150, 0, 0);
}

int isButtonSelected(gameManager_t *gm)
{
    /* 
    largura do botao: 210
    altura do botao: 160
    */

    if (gm->mouseX >= 160 && gm->mouseX <= 370){
        if (gm->mouseY >= 530 && gm->mouseY <= 690){
            return 4; /* IN_GAME */
        }
    } else if (gm->mouseX >= 400 && gm->mouseX <= 610){
        if (gm->mouseY >= 530 && gm->mouseY <= 690){
            return 2; /* TOP_SCORE */
        }
    } else if (gm->mouseX >= 640 && gm->mouseX <= 850){
        if (gm->mouseY >= 530 && gm->mouseY <= 690){
            return 3; /* CREDITS_MENU */
        }
    } else if (gm->mouseX >= 880 && gm->mouseX <= 1090){
        if (gm->mouseY >= 530 && gm->mouseY <= 690){
            return 5; /* EXIT */
        }
    }
    return 1; /* MAIN_MENU */
}

/* ------------- Funcoes globais -------------*/

void runMenu(gameManager_t *gm)
{
    al_register_event_source(gm->evQueue, al_get_timer_event_source(gm->timer));
    al_register_event_source(gm->evQueue, al_get_display_event_source(gm->disp));
    al_register_event_source(gm->evQueue, al_get_keyboard_event_source());
    al_register_event_source(gm->evQueue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT ev;

    al_start_timer(gm->timer);

    gm->menuState = MAIN_MENU;

    /* Loop dos menus */
    while (1)
    {
        al_wait_for_event(gm->evQueue, &ev);

        switch (ev.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                switch (ev.keyboard.keycode)
                {
                    case ALLEGRO_KEY_ESCAPE:
                        done = true;
                        break;

                    case ALLEGRO_KEY_ENTER:
                        gm->menuState = IN_GAME;
                        break;
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                gm->mouseX = ev.mouse.x;
                gm->mouseY = ev.mouse.y;

                /* casos para o menu principal */
                if (gm->menuState == MAIN_MENU){
                    switch (isButtonSelected(gm)){
                    case 1:
                        gm->menuState = MAIN_MENU; 
                        break;
                    case 2:
                        gm->menuState = TOPSCORE_MENU;
                        break;
                    case 3:
                        gm->menuState = CREDITS_MENU;
                        break;
                    case 4:
                        gm->menuState = IN_GAME;
                        break;
                    case 5:
                        gm->menuState = EXIT;
                        done = 1;
                        break;
                    }
                }

                /* casos para sair de pop-ups */
                if (gm->menuState == TOPSCORE_MENU){
                    if (gm->mouseX >= 336 && gm->mouseX <= 452){
                        if (gm->mouseY >= 79 && gm->mouseY <= 120){
                            gm->menuState = MAIN_MENU;
                        }
                    }
                }

                if (gm->menuState == CREDITS_MENU){
                    if (gm->mouseX >= 285 && gm->mouseX <= 415){
                        if (gm->mouseY >= 103 && gm->mouseY <= 150){
                            gm->menuState = MAIN_MENU;
                        }
                    }
                }

                break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(gm->evQueue))
        {
            switch (gm->menuState){
                case MAIN_MENU:
                    drawMainMenu(gm);
                    break;

                case TOPSCORE_MENU:
                    drawMainMenu(gm);
                    drawTopScoreUI(gm);
                    break;

                case CREDITS_MENU:
                    drawMainMenu(gm);
                    drawCreditsUI(gm);
                    break;

                case IN_GAME:
                    drawMainMenu(gm);
                    runGame(gm);
                    break;
                
                case EXIT:
                    done = 1;
                    break;
            }

            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), 100, 140, 0, "logicState: %d, menuState: %d", gm->logicState, gm->menuState);
            al_flip_display();
            redraw = false;
        }
    }
}