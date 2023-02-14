#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "init.h"
#include "structs.h"

/* ------------- Prototypes ------------- */
void mustInit(bool test, const char *description);
void initAll(gameManager_t *gm);
void loadBitmaps(gameManager_t *gm);

/* ------------- Funcoes internas -------------*/

/* Função que verifica se algo foi iniciado corretamente */
void mustInit(bool test, const char *description)
{
    if (test)
        return;

    printf("Couldn't initialize %s\n", description);
    exit(1);
}

void loadAddons(gameManager_t *gm)
{
    /* Carrega os sprites dos peixes */
    gm->fishes[FISH_0] = al_load_bitmap("addons/images/fish0.png");
    gm->fishes[FISH_1] = al_load_bitmap("addons/images/fish1.png");
    gm->fishes[FISH_2] = al_load_bitmap("addons/images/fish2.png");
    gm->fishes[FISH_3] = al_load_bitmap("addons/images/fish3.png");
    gm->fishes[FISH_4] = al_load_bitmap("addons/images/fish4.png");

    /* Carrega as imagens de fundo */
    gm->backgrounds[BACKGROUND_GAME] = al_load_bitmap("addons/images/background_game.jpg");

    /* Carrega as outras imagens */
    gm->otherBitmaps[OBJECTIVE_BOARD] = al_load_bitmap("addons/images/objectiveBoard.png");
    gm->otherBitmaps[SCORE_BOARD] = al_load_bitmap("addons/images/scoreBoard.png");

    /* Carrega as fontes */
}

/* ------------- Funcoes globais -------------*/

void initAllegro()
{
    /* inicialização do allegro */
    mustInit(al_init(), "allegro");

    /* inicialização das extensões do allegro */
    mustInit(al_init_image_addon(), "image_addon");
    mustInit(al_init_acodec_addon(), "acodec_addon");
    mustInit(al_init_ttf_addon(), "ttf_addon");
    mustInit(al_install_audio(), "audio");
    mustInit(al_install_mouse(), "mouse");
    mustInit(al_install_keyboard(), "keyboard");
}

gameManager_t *initGameManager()
{
    /* aloca memoria para o gm */
    gameManager_t *gm = (gameManager_t *) malloc(sizeof(gameManager_t));

    /* inicializa o display */
    gm->disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    mustInit(gm->disp, "display");

    al_set_window_title(gm->disp, "Fishing Crush");

    /* inicializa a fila de eventos */
    gm->evQueue = al_create_event_queue();
    mustInit(gm->evQueue, "evQueue");

    /* aloca a matriz de peixes */
    gm->matrix = malloc(sizeof(fish_t *) * MATRIX_SIZE);

    int i;
    for (i = 0; i < MATRIX_SIZE; i++)
        gm->matrix[i] = malloc(sizeof(fish_t) * MATRIX_SIZE);

    /* carrega os addons do jogo */ 
    loadAddons(gm);

    return gm;
}