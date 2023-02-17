#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "update.h"
#include "structs.h"

/* ------------- Funcoes internas -------------*/

void switchFishes(gameManager_t *gm, fish_t *fish1, fish_t *fish2)
{
    return;
}

void removeMatched(gameManager_t *gm)
{
    return;
}

bool checkMatch(gameManager_t *gm)
{
    return false;
}

/* ------------- Funcoes globais -------------*/

bool isFishSelected(gameManager_t *gm)
{
    /* checa toda matriz para ver se o mouseX e mouseY estao em algum fishX fishY */
    int i, j;

    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (! gm->matrix[i][j]->selected){
                if (gm->mouseX >= gm->matrix[i][j]->fishX && gm->mouseX <= gm->matrix[i][j]->fishX + 60 &&
                gm->mouseY >= gm->matrix[i][j]->fishY && gm->mouseY <= gm->matrix[i][j]->fishY + 60){
                    return true;
                }
            }
        }
    }

}

/* Atualiza a LOGICA */
void updateLogic(gameManager_t *gm)
{   
    bool done = false;

    while (! done)
    {
        switch (gm->logicState){
            case SWITCHING:
                gm->logicState = PROCESSING;
                break;
            
            case PROCESSING:
                gm->logicState = STANDBY;
                break;

            case HELP_WD:
                done = true;
                break;

            case STANDBY:
                done = true;
                break;
        }  
    }
}