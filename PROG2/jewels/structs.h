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
#define FPS 60.0

enum GameState {
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
    FISH_4,
    FISH_0_SELECTED,
    FISH_1_SELECTED,
    FISH_2_SELECTED,
    FISH_3_SELECTED,
    FISH_4_SELECTED,
};

enum FishTypes {
    FISH_TYPE_0,
    FISH_TYPE_1,
    FISH_TYPE_2,
    FISH_TYPE_3,
    FISH_TYPE_4,
    FISH_TYPE_NULL
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
    TITLE_FONT,
    DIALOGUE_FONT
};

typedef struct fish {
    int fishID;
    int fishType;
    int fishX;
    int fishY;
    int fishMoveX;
    int fishMoveY;
    int selected;
    int matI;
    int matJ;
    ALLEGRO_BITMAP *sprite[2];
} fish_t;

typedef struct gameManager {
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_BITMAP *fishes[10];
    ALLEGRO_BITMAP *backgrounds[2];
    ALLEGRO_BITMAP *otherBitmaps[5];
    ALLEGRO_FONT *fonts[3];
    ALLEGRO_EVENT_QUEUE *evQueue;
    ALLEGRO_TIMER *timer;
    fish_t ***matrix;
    fish_t *selectedFishes[2];
    int mouseX;
    int mouseY;
    int selected;
    int gameState;
} gameManager_t;

#endif