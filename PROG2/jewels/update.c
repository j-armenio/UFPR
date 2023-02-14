#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "update.h"
#include "structs.h"

/* ------------- Prototypes ------------- */
void checkMatch(gameManager_t *gm);
void updateLogic(gameManager_t *gm);
bool isFishSelected(gameManager_t *gm);

/* ------------- Funcoes internas -------------*/

/* ------------- Funcoes globais -------------*/

bool isFishSelected(gameManager_t *gm)
{
    /*
    Preciso pegar as coordenadas do mouse e comparar com as coordenadas de cada peixe da matriz,
    caso ela seja igual a alguma, eu ligo o selected no peixe e retorno true 
    */
    int i, j;

    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->mouseX >= gm->matrix[i][j].fishX && gm->mouseX <= gm->matrix[i][j].fishX + 60 &&
                gm->mouseY >= gm->matrix[i][j].fishY && gm->mouseY <= gm->matrix[i][j].fishY + 60){
                    gm->matrix[i][j].selected = true;
                    return true;
                }
        }
    }
    return false;
}

void checkMatch(gameManager_t *gm)
{ }

/* Atualiza a LOGICA */
void updateLogic(gameManager_t *gm)
{ }