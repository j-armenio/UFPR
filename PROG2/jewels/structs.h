#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720
#define MATRIX_SIZE 7
#define OFFSET_X 690
#define OFFSET_Y 125
#define BETWEEN_FISHES 80
#define FPS 60.0
#define MOVE_SPEED 10

/* Estados de Menu */
enum MenuStates {
    MAIN_MENU,
    TOPSCORE_MENU,
    CREDITS_MENU,
    IN_GAME,
    EXIT
};

/* Estados de jogo */
enum LogicState {
    STANDBY,
    SWITCHING,
    PROCESSING,
    HELP_WD
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
    FISH_NULL
};

enum FishTypes {
    FISH_TYPE_0,
    FISH_TYPE_1,
    FISH_TYPE_2,
    FISH_TYPE_3,
    FISH_TYPE_4,
    FISH_TYPE_NULL
};

enum Neighbours {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NEIGHBOURS_NULL
};

enum BitmapBackgrounds {
    START_SCREEN,
    BACKGROUND_GAME
};

enum OthersBitmaps {
    SCORE_BOARD,
    OBJECTIVE_BOARD,
    TOP_SCORE_UI,
    CREDITS_UI,
    HELP_UI
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
    int matched;
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
    int switchNeighbour;
    int switchI;
    int switchDone;
    int logicState;
    int menuState;
} gameManager_t;

#endif