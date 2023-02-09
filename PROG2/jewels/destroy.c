/* Funções que liberam a memória alocada */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "destroy.h"
#include "init.h"

void freeAll(ALLEGRO_DISPLAY *disp)
{
    /* al_destroy_bitmap(background); */
    /* al_destroy_font() */
    al_destroy_display(disp);
}