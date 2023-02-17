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
            /* FISH INFO*/

            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), gm->matrix[i][j]->fishX, gm->matrix[i][j]->fishY, 0, "x:%dy:%d", gm->matrix[i][j]->fishX, gm->matrix[i][j]->fishY);
            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), gm->matrix[i][j]->fishX, gm->matrix[i][j]->fishY+20, 0, "matched:%d", gm->matrix[i][j]->matched);
            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), gm->matrix[i][j]->fishX, gm->matrix[i][j]->fishY+40, 0, "selected:%d", gm->matrix[i][j]->selected);
            al_draw_textf(gm->fonts[DIALOGUE_FONT], al_map_rgb(255, 255, 255), gm->matrix[i][j]->fishX, gm->matrix[i][j]->fishY+60, 0, "fishType:%d", gm->matrix[i][j]->fishType);

            aux2 += 80;
        }
        aux1 += 80;
        aux2 = 0;
    }

    /* caso esteja com o help aberto */
    if (gm->logicState == HELP_WD)
                al_draw_bitmap(gm->otherBitmaps[HELP_UI], 7, 10, 0);
}