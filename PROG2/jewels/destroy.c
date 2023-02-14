#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "destroy.h"

void freeAll(gameManager_t *gm)
{
    al_destroy_display(gm->disp);
    al_destroy_timer(gm->timer);
    al_destroy_event_queue(gm->evQueue);

    free(gm);
}