#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "init.h"
#include "structs.h"

/* ------------- Funcoes internas -------------*/

/* Função que verifica se algo foi iniciado corretamente */
void mustInit(bool test, const char *description)
{
    if (test)
        return;

    printf("Couldn't initialize %s\n", description);
    exit(1);
}

int getRandomFish()
{
    int ID = rand() % 5;

    if (ID == 0)
        return FISH_0;
    else if (ID == 1)
        return FISH_1;
    else if (ID == 2)
        return FISH_2;
    else if (ID == 3)
        return FISH_3;
    else
        return FISH_4;
}

void loadAddons(gameManager_t *gm)
{
    /* Carrega os sprites dos peixes */
    gm->fishes[FISH_0] = al_load_bitmap("addons/images/fish0.png");
    gm->fishes[FISH_1] = al_load_bitmap("addons/images/fish1.png");
    gm->fishes[FISH_2] = al_load_bitmap("addons/images/fish2.png");
    gm->fishes[FISH_3] = al_load_bitmap("addons/images/fish3.png");
    gm->fishes[FISH_4] = al_load_bitmap("addons/images/fish4.png");
    gm->fishes[FISH_0_SELECTED] = al_load_bitmap("addons/images/fish0_selected.png");
    gm->fishes[FISH_1_SELECTED] = al_load_bitmap("addons/images/fish1_selected.png");
    gm->fishes[FISH_2_SELECTED] = al_load_bitmap("addons/images/fish2_selected.png");
    gm->fishes[FISH_3_SELECTED] = al_load_bitmap("addons/images/fish3_selected.png");
    gm->fishes[FISH_4_SELECTED] = al_load_bitmap("addons/images/fish4_selected.png");

    /* Carrega as imagens de fundo */
    gm->backgrounds[START_SCREEN] = al_load_bitmap("addons/images/start_screen.jpg");
    gm->backgrounds[BACKGROUND_GAME] = al_load_bitmap("addons/images/background_game.jpg");

    /* Carrega as outras imagens */
    gm->otherBitmaps[OBJECTIVE_BOARD] = al_load_bitmap("addons/images/objectiveBoard.png");
    gm->otherBitmaps[SCORE_BOARD] = al_load_bitmap("addons/images/scoreBoard.png");
    gm->otherBitmaps[TOP_SCORE_UI] = al_load_bitmap("addons/images/topscore.png");
    gm->otherBitmaps[CREDITS_UI] = al_load_bitmap("addons/images/credits.png");
    gm->otherBitmaps[HELP_UI] = al_load_bitmap("addons/images/help_me.png");

    /* Carrega as fontes */
    gm->fonts[MAIN_FONT] = al_load_ttf_font("addons/fonts/StVlRegular.ttf", 20, 0);
    gm->fonts[TITLE_FONT] = al_load_ttf_font("addons/fonts/StVlAllCaps.ttf", 20, 0);
    gm->fonts[DIALOGUE_FONT] = al_load_ttf_font("addons/fonts/StVlDialogue.ttf", 20, 0);
}

/* Inicializa apenas a parte logica da matriz */
void startMatrix(gameManager_t *gm)
{
    /* gera um peixe para cada espaço da matriz */
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            gm->matrix[i][j]->fishID = getRandomFish();
            gm->matrix[i][j]->selected = 0;
            gm->matrix[i][j]->matched = 0;
            gm->matrix[i][j]->fishMoveX = 0;
            gm->matrix[i][j]->fishMoveY = 0;
        }
    }

    /* preenche os componentes de cada peixe dependendo de seu ID */
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->matrix[i][j]->fishID == FISH_0){
                gm->matrix[i][j]->sprite[0] = gm->fishes[FISH_0];
                gm->matrix[i][j]->sprite[1] = gm->fishes[FISH_0_SELECTED];
                gm->matrix[i][j]->fishType = FISH_TYPE_0;
            }
            else if (gm->matrix[i][j]->fishID == FISH_1){
                gm->matrix[i][j]->sprite[0] = gm->fishes[FISH_1];
                gm->matrix[i][j]->sprite[1] = gm->fishes[FISH_1_SELECTED];
                gm->matrix[i][j]->fishType = FISH_TYPE_1;
            }
            else if (gm->matrix[i][j]->fishID == FISH_2){
                gm->matrix[i][j]->sprite[0] = gm->fishes[FISH_2];
                gm->matrix[i][j]->sprite[1] = gm->fishes[FISH_2_SELECTED];
                gm->matrix[i][j]->fishType = FISH_TYPE_2;
            }
            else if (gm->matrix[i][j]->fishID == FISH_3){
                gm->matrix[i][j]->sprite[0] = gm->fishes[FISH_3];
                gm->matrix[i][j]->sprite[1] = gm->fishes[FISH_3_SELECTED];
                gm->matrix[i][j]->fishType = FISH_TYPE_3;
            }
            else if (gm->matrix[i][j]->fishID == FISH_4){
                gm->matrix[i][j]->sprite[0] = gm->fishes[FISH_4];
                gm->matrix[i][j]->sprite[1] = gm->fishes[FISH_4_SELECTED];
                gm->matrix[i][j]->fishType = FISH_TYPE_4;
            }
            gm->matrix[i][j]->matI = i;
            gm->matrix[i][j]->matJ = j;
        }
    }

    int aux1, aux2;
    aux1 = aux2 = 0;

    /* preenche os fishX e fishY dos peixes */
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            gm->matrix[i][j]->fishX = OFFSET_X + aux1;
            gm->matrix[i][j]->fishY = OFFSET_Y + aux2;
            aux2 += 80;
        }
        aux1 += 80;
        aux2 = 0;
    }
}

/* ------------- Funcoes globais -------------*/

void initAllegro()
{
    /* inicialização do allegro */
    mustInit(al_init(), "allegro");

    /* inicialização das extensões do allegro */
    mustInit(al_init_image_addon(), "image_addon");
    mustInit(al_init_acodec_addon(), "acodec_addon");
    mustInit(al_init_font_addon(), "font_addon");
    mustInit(al_init_ttf_addon(), "ttf_addon");
    mustInit(al_install_audio(), "audio");
    mustInit(al_install_mouse(), "mouse");
    mustInit(al_install_keyboard(), "keyboard");
}

gameManager_t *initGameManager()
{
    /* aloca memoria e inicializa os componentes de gm */
    gameManager_t *gm = (gameManager_t *) malloc(sizeof(gameManager_t));

    gm->disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    mustInit(gm->disp, "display");
    al_set_window_title(gm->disp, "Fish Valley");

    gm->evQueue = al_create_event_queue();
    mustInit(gm->evQueue, "evQueue");

    gm->timer = al_create_timer(1.0 / FPS);
    mustInit(gm->timer, "timer");

    /* aloca a matriz e os peixes */
    int i, j;
    gm->matrix = malloc(sizeof(fish_t *) * MATRIX_SIZE);
    for (i = 0; i < MATRIX_SIZE; i++)
        gm->matrix[i] = malloc(sizeof(fish_t) * MATRIX_SIZE);

    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            gm->matrix[i][j] = malloc(sizeof(fish_t));
        }
    }

    gm->mouseX = gm->mouseY = 0;
    gm->logicState = STANDBY;
    gm->menuState = MAIN_MENU;
    gm->switchNeighbour = NEIGHBOURS_NULL;
    gm->switchDone = 0;
    gm->switchI = 0;

    for (i = 0; i < 1; i++)
        gm->selectedFishes[i] = NULL;
    gm->selected = 0;

    loadAddons(gm);
    startMatrix(gm);

    return gm;
}