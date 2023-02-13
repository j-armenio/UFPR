#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "init.h"
#include "structs.h"

/* Prototypes */
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

void loadBitmaps(gameManager_t *gm)
{
    gm->backgrounds[BACKGROUND_GAME] = al_load_bitmap("addons/images/background_game.jpg");
}

/* ------------- Funcoes externas -------------*/

void initAll(gameManager_t *gm)
{
    gm = malloc(sizeof(gameManager_t));

    /* inicialização do allegro */
    mustInit(al_init(), "allegro");

    /* inicialização das extensões do allegro */
    mustInit(al_init_image_addon(), "image_addon");
    mustInit(al_init_acodec_addon(), "acodec_addon");
    mustInit(al_init_ttf_addon(), "ttf_addon");
    mustInit(al_install_audio(), "audio");
    mustInit(al_install_mouse(), "mouse");
    mustInit(al_install_keyboard(), "keyboard");

    gm->disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    mustInit(gm->disp, "display");

    al_set_window_title(gm->disp, "Jewels");

    /* carregando addons */
    loadBitmaps(gm);

    al_draw_bitmap(gm->backgrounds[BACKGROUND_GAME], 0, 0, 0);

    al_flip_display();
    al_rest(30.0);
}