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
    int auxI, auxJ;

    auxI = fish1->matI;
    auxJ = fish1->matJ;

    fish1->matI = fish2->matI;
    fish1->matJ = fish2->matJ;

    fish2->matI = auxI;
    fish2->matJ = auxJ;

    gm->matrix[fish1->matI][fish1->matJ] = fish1;
    gm->matrix[fish2->matI][fish2->matJ] = fish2;
}

void removeMatched(gameManager_t *gm)
{
    int i, j;

    /* remove da matriz todos peixes com matched = 1 */
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->matrix[i][j]->matched == 1){
                gm->matrix[i][j]->fishType = FISH_TYPE_NULL;
                gm->matrix[i][j]->matched = 0;
                gm->matrix[i][j]->fishID = FISH_NULL;
            }
        }
    }
}

bool checkMatch(gameManager_t *gm)
{
    /* percorre toda matriz procurando por um match */
    int i, j;
    int match = 0;

    /* percorre a matriz e caso tenham 3, 4 ou 5 peixes iguais em linha ou coluna todos são marcados para serem removidos. */
    for (i = 0; i < MATRIX_SIZE; i++)
    {
        for (j = 0; j < MATRIX_SIZE; j++)
        {
            /* checa match horizontal */
            if (i < MATRIX_SIZE - 2)
            {
                if ((gm->matrix[i][j]->fishType == gm->matrix[i+1][j]->fishType) &&
                    (gm->matrix[i+1][j]->fishType == gm->matrix[i+2][j]->fishType)){
                        gm->matrix[i][j]->matched = 1;
                        gm->matrix[i+1][j]->matched = 1;
                        gm->matrix[i+2][j]->matched = 1;
                        match = 1;
                }
            }
            /* checa math vertical */
            if (j < MATRIX_SIZE - 2)
            {
                if ((gm->matrix[i][j]->fishType == gm->matrix[i][j+1]->fishType) &&
                    (gm->matrix[i][j+1]->fishType == gm->matrix[i][j+2]->fishType)){
                        gm->matrix[i][j]->matched = 1;
                        gm->matrix[i][j+1]->matched = 1;
                        gm->matrix[i][j+2]->matched = 1;
                        match = 1;
                }
            }
        }
    }

    if (match == 1)
        return true;
    else
        return false;
}

/* ------------- Funcoes globais -------------*/

bool isFishSelected(gameManager_t *gm)
{
    int i, j;

    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->mouseX >= gm->matrix[i][j]->fishX && gm->mouseX <= gm->matrix[i][j]->fishX + 60 &&
                gm->mouseY >= gm->matrix[i][j]->fishY && gm->mouseY <= gm->matrix[i][j]->fishY + 60){
                    gm->matrix[i][j]->selected = true;
                    gm->selectedFishes[gm->selected] = gm->matrix[i][j];
                    return true;
                }
        }
    }
    return false;
}

/* Atualiza a LOGICA */
void updateLogic(gameManager_t *gm)
{   
    bool done = false;

    if (gm->selected == 2)
        gm->logicState = SWITCHING;
    else if (gm->logicState == STANDBY)
        gm->logicState = PROCESSING;
    else
        gm->logicState = STANDBY;

    while (! done)
    {
        switch (gm->logicState){
            case SWITCHING:
                switchFishes(gm, gm->selectedFishes[0], gm->selectedFishes[1]);
                gm->selectedFishes[0]->selected = false;
                gm->selectedFishes[1]->selected = false;
                gm->selected = 0;
                gm->logicState = PROCESSING;
                break;
            
            case PROCESSING:
                if (checkMatch(gm)){
                    removeMatched(gm);
                } else {
                    switchFishes(gm, gm->selectedFishes[0], gm->selectedFishes[1]);
                    gm->selectedFishes[0]->selected = false;
                    gm->selectedFishes[1]->selected = false;
                    gm->selected = 0;
                }
                gm->logicState = STANDBY;
                break;
            
            case STANDBY:
                done = true;
                break;
        }  
    }
}