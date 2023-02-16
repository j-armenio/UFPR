#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "draw.h"
#include "structs.h"

/* ------------- Funcoes internas ------------- */


/* ------------- Funcoes globais -------------*/

/* Atualiza A PARTE VISUAL */
void updateVisual(gameManager_t *gm)
{
    int i, j;

    al_draw_bitmap(gm->backgrounds[BACKGROUND_GAME], 0, 0, 0);

    /* desenha os peixes na tela */
    int aux1, aux2;
    aux1 = aux2 = 0;
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->matrix[i][j]->fishType == FISH_TYPE_NULL)
                continue;
            else if (gm->matrix[i][j]->selected){
                al_draw_bitmap(gm->matrix[i][j]->sprite[1], OFFSET_X + aux1, OFFSET_Y + aux2, 0);
            } else {
                al_draw_bitmap(gm->matrix[i][j]->sprite[0], OFFSET_X + aux1, OFFSET_Y + aux2, 0);
            }
            gm->matrix[i][j]->fishX = OFFSET_X + aux1;
            gm->matrix[i][j]->fishY = OFFSET_Y + aux2;
            aux2 += 80;
        }
        aux1 += 80;
        aux2 = 0;
    }
}