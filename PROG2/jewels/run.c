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

void runGame(gameManager_t *gm)
{
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT ev;

    /* Loop principal do jogo */
    while (1)
    {
        al_wait_for_event(gm->evQueue, &ev);

        switch (ev.type)
        {
            case ALLEGRO_EVENT_TIMER:
                updateLogic(gm);

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

                    case ALLEGRO_KEY_H:
                        if (gm->logicState == STANDBY){
                            gm->logicState = HELP_WD;
                            redraw = true;
                        } else if (gm->logicState == HELP_WD){
                            gm->logicState = STANDBY;
                            redraw = true;
                        }

                        break;
                    case ALLEGRO_KEY_F1:
                        if (gm->logicState == STANDBY){
                            gm->logicState = HELP_WD;
                            redraw = true;
                        } else if (gm->logicState == HELP_WD){
                            gm->logicState = STANDBY;
                            redraw = true;
                        }

                        break;
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                gm->mouseX = ev.mouse.x;
                gm->mouseY = ev.mouse.y;

                if (gm->logicState == STANDBY){
                    if (isFishSelected(gm)){
                        
                    }

                    /* Verifica se clicou no botao BACK */
                    if (gm->mouseX >= 222 && gm->mouseX <= 352){
                        if (gm->mouseY >= 460 && gm->mouseY <= 507){
                            done = true;
                        }
                    }
                }

                /* Check if exit button is pressed in HELP */
                if (gm->logicState == HELP_WD){
                    if (gm->mouseX >= 94 && gm->mouseX <= 208){
                        if (gm->mouseY >= 90 && gm->mouseY <= 130){
                            gm->logicState = STANDBY;
                            redraw = true;
                        }
                    }
                }

                break;
        }

        if (done){
            gm->menuState = MAIN_MENU;
            break;
        }

        if (redraw && al_is_event_queue_empty(gm->evQueue))
        {
            updateVisual(gm);
    
            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), 100, 100, 0, "Mouse X: %d, Mouse Y: %d", gm->mouseX, gm->mouseY);
            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), 100, 120, 0, "selected: %d", gm->selected);
            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), 100, 140, 0, "logicState: %d, menuState: %d", gm->logicState, gm->menuState);
            al_flip_display();
            redraw = false;
        }
    }
}