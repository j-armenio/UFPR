#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#define MATRIX_SIZE 7
#define OFFSET_X 690
#define OFFSET_Y 125
#define BETWEEN_FISHES 80

enum State {
    STARTING, 
    STANDBY, 
    PLAYING, 
    ENDPHASE
};

enum BitmapFishes {
    FISH_0,
    FISH_1,
    FISH_2,
    FISH_3,
    FISH_4
};

enum BitmapBackgrounds {
    BACKGROUND_MENU,
    BACKGROUND_GAME
};

enum OthersBitmaps {
    SCORE_BOARD,
    OBJECTIVE_BOARD
};

enum Fonts {
    MAIN_FONT, 
    TITLE_FONT
};

typedef struct fish {
    int fishID;
    ALLEGRO_BITMAP *sprite;
} fish_t;

typedef struct gameManager {
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_BITMAP *fishes[5];
    ALLEGRO_BITMAP *backgrounds[2];
    ALLEGRO_BITMAP *otherBitmaps[5];
    ALLEGRO_EVENT_QUEUE *evQueue;
    fish_t **matrix;
} gameManager_t;

#endif