#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "run.h"
#include "structs.h"
#include "draw.h"

void runGame(gameManager_t *gm)
{
    bool done = false;

    while (! done)
    {
        drawFirstGame(gm);

        al_flip_display();
        al_rest(50.0);
        done = true;
    }
}