#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "init.h"
#include "structs.h"
#include "destroy.h"
#include "draw.h"
#include "run.h"
#include "menu.h"

int main()
{
    srand(time(NULL));

    gameManager_t *gm = NULL;

    /* Inicializa o Allegro, suas extensoes e o gameManager */
    initAllegro();
    gm = initGameManager();

    /* maquina de estados para menu do jogo */
    runMenu(gm);

    /* Libera toda memoria alocada */
    freeAll(gm);
}