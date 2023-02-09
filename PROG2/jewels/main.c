#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "init.h"
#include "draw.h"
#include "structs.h"
#include "destroy.h"

int main()
{
    /* Variáveis do Allegro */
    ALLEGRO_DISPLAY *disp = NULL;
    ALLEGRO_EVENT_QUEUE *evQueue = NULL;
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_FONT *titleFont = NULL;
    ALLEGRO_FONT *mainFont = NULL;

    /* Variáveis */

    /* Inicializa o Allegro e suas extensões */
    initAll(disp, evQueue);

    loadFiles(background, titleFont, mainFont);

    drawLevel();

    /* Libera toda memória alocada */
    freeAll(disp);
}