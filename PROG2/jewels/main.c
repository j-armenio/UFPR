#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "init.h"
#include "structs.h"
#include "destroy.h"

int main()
{
    gameManager_t *gm = NULL;

    /* Inicializa o gameManager, Allegro e suas extensoes*/
    initAll(gm);

    /* Desenha a matriz e atribui uma crop para cada espaço na matriz */
    /* drawGame(gm); */

    /* Libera toda memoria alocada */
    freeAll(gm);
}