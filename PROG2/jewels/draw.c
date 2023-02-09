#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "draw.h"
#include "init.h"

int loadFiles(ALLEGRO_BITMAP *background, ALLEGRO_FONT *titleFont, ALLEGRO_FONT *mainFont)
{
    background = al_load_bitmap("stardew_bg.jpg");
    mustInit(background, "addons/background");
    
    titleFont = al_load_font("addons/StVlAllCaps.ttf", 42, 0);
    mustInit(titleFont, "titleFont");

    mainFont = al_load_font("addons/StVlRegular.ttf", 42, 0);
    mustInit(mainFont, "main");

    return 0;
}

int drawLevel()
{
    
}