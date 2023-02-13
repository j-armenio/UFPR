#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#define MAT_ROWS 8
#define MAT_COLS 8

enum State {
    STARTING, 
    STANDBY, 
    PLAYING, 
    ENDPHASE
};

enum BitmapCrops {
    CROP_0,
    CROP_1,
    CROP_2,
    CROP_3,
    CROP_4
};

enum BitmapBackgrounds {
    BACKGROUND_MENU,
    BACKGROUND_GAME
};

enum Fonts {
    MAIN_FONT, 
    TITLE_FONT
};

typedef struct gameManager {
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_BITMAP *crops[5];
    ALLEGRO_BITMAP *backgrounds[2];
} gameManager_t;

typedef struct crop {
    int crop;
    ALLEGRO_BITMAP *img;
} crop_t;

#endif