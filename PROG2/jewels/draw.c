#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "draw.h"
#include "structs.h"

/* ------------- Prototypes ------------- */
void drawFirstGame(gameManager_t *gm);

/* ------------- Funcoes internas ------------- */

int getRandomFish()
{
    int ID = rand() % 5;

    if (ID == 0)
        return FISH_0;
    else if (ID == 1)
        return FISH_1;
    else if (ID == 2)
        return FISH_2;
    else if (ID == 3)
        return FISH_3;
    else
        return FISH_4;
}

/* ------------- Funcoes globais -------------*/

void drawFirstGame(gameManager_t *gm)
{
    srand(time(NULL));

    al_draw_bitmap(gm->backgrounds[BACKGROUND_GAME], 0, 0, 0);

    /* gera um peixe para cada espaço da matriz */
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            gm->matrix[i][j].fishID = getRandomFish();
        }
    }

    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->matrix[i][j].fishID == FISH_0)
                gm->matrix[i][j].sprite = gm->fishes[FISH_0];
            else if (gm->matrix[i][j].fishID == FISH_1)
                gm->matrix[i][j].sprite = gm->fishes[FISH_1];
            else if (gm->matrix[i][j].fishID == FISH_2)
                gm->matrix[i][j].sprite = gm->fishes[FISH_2];
            else if (gm->matrix[i][j].fishID == FISH_3)
                gm->matrix[i][j].sprite = gm->fishes[FISH_3];
            else if (gm->matrix[i][j].fishID == FISH_4)
                gm->matrix[i][j].sprite = gm->fishes[FISH_4];
        }
    }

    /* desenha os peixes na tela */
    int aux1, aux2;
    aux1 = aux2 = 0;
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            al_draw_bitmap(gm->matrix[i][j].sprite, OFFSET_X + aux1, OFFSET_Y + aux2, 0);
            aux2 += 80;
        }
        aux1 += 80;
        aux2 = 0;
    }

    /* Desenha o resto da UI */
    al_draw_bitmap(gm->otherBitmaps[SCORE_BOARD], 40, 10, 0);
    al_draw_bitmap(gm->otherBitmaps[OBJECTIVE_BOARD], 20, 200, 0);

}