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


bool checkMatch(gameManager_t *gm)
{

    return true;
    /* percorre toda matriz procurando por um match */
    int i, j;
    int match = 0;

    /* varrer a matriz toda checando os tipos horizontalmente e verticalmente
    caso ache um match de 3, 4 ou 5, destruir todos os peixes */
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            /* checa horizontalmente */
            if (gm->matrix[i][j]->fishType == gm->matrix[i][j+1]->fishType &&
                gm->matrix[i][j]->fishType == gm->matrix[i][j+2]->fishType){
                    match++;
                    gm->matrix[i][j]->matched = true;
                    gm->matrix[i][j+1]->matched = true;
                    gm->matrix[i][j+2]->matched = true;
                }
            /* checa verticalmente */
        }
    }

    return true;
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
    /* 
    Fazer uma maquina de estados para cada fase do processo 
    STANDBY - se selected == 2, troca os peixes
    PROCESSING - checa se tem match, se tiver match, remove os peixes / se nao tiver match, troca os peixes de volta
    */

    bool done = false;

    if (gm->selected == 2)
        gm->logicState = SWITCHING;
    else if (gm->logicState == STANDBY)
        gm->logicState = PROCESSING;

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
                    /* HOUVE MATCH, remove os peixes que deram match */
                } else {
                    /* NÃO HOUVE MATCH, retorna os selecionados para sua posição original */
                }
                gm->logicState = STANDBY;
                break;
            
            case STANDBY:
                done = true;
                break;
        }  
    }
}