/* Funções que inicializam o Allegro e todas suas extensões */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "init.h"

/* Função que verifica se algo foi iniciado corretamente */
void mustInit(bool test, const char *description)
{
    if (test) 
        return;

    printf("Couldn't initialize %s\n", description);
    exit(1);
}

/* Inicializa as dependencias e cria o display */
void initAll(ALLEGRO_DISPLAY *disp, ALLEGRO_EVENT_QUEUE *evQueue)
{
    /* Inicialização do Allegro */
    mustInit(al_init(), "allegro");

    /* Inicialização das extensões do Allegro */
    mustInit(al_init_image_addon(), "image_addon");
    mustInit(al_install_audio(), "audio");
    mustInit(al_init_acodec_addon(), "acodec_addon");
    mustInit(al_init_ttf_addon(), "ttf_addon");

    /* Cria o display */
    disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    mustInit(disp, "display");

    evQueue = al_create_event_queue();
    mustInit(evQueue, "evQueue");

    al_set_window_title(disp, "Jewels: Stardew Crops");

    al_register_event_source(evQueue, al_get_display_event_source(disp));

    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_flip_display();
    al_rest(10.0);
}