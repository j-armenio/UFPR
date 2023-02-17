#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "update.h"
#include "structs.h"

/* ------------- Funcoes internas -------------*/

/* Checa as possibilidade do que o curFish eh */
bool isNeighbor(gameManager_t *gm, fish_t *curFish)
{
    fish_t *prevFish = gm->selectedFishes[0];

    /* posicoes em relacao ao cruFish*/
    if (curFish->fishX == prevFish->fishX){ /* estao no mesmo eixo x, ou ta em cima ou embaixo */
        if (curFish->fishY - prevFish->fishY == -80){ /* eh o vizinho de cima */
            gm->switchNeighbour = UP;
            return true;
        }   
        if (curFish->fishY - prevFish->fishY == 80){ /* eh o vizinho de baixo */
            gm->switchNeighbour = DOWN;
            return true;
        } 
    }
    if (curFish->fishY == prevFish->fishY){ /* estao no mesmo eixo x, ou ta na esquerda ou direita */
        if (curFish->fishX - prevFish->fishX == -80){ /* eh o vizinho da esquerda */
            gm->switchNeighbour = LEFT;
            return true;
        } 
        if (curFish->fishX - prevFish->fishX == 80){ /* eh o vizinho da direita */
            gm->switchNeighbour = RIGHT;
            return true;
        } 
    }

    return false;
}

void switchFishes(gameManager_t *gm)
{   
    fish_t *fish1 = gm->selectedFishes[0];
    fish_t *fish2 = gm->selectedFishes[1];
    int auxX, auxY;

    auxX = fish1->fishX;
    auxY = fish1->fishY;

    fish1->fishX = fish2->fishX;
    fish1->fishY = fish2->fishY;
    
    fish2->fishX = auxX;
    fish2->fishY = auxY;

    fish1->selected = false;
    fish2->selected = false;

    gm->selected = 0;
    gm->logicState = PROCESSING;
}

void checkAndRemoveMatches(gameManager_t *gm) 
{

}

void moveToBottom(gameManager_t *gm)
{


}

void fillBlank(gameManager_t *gm)
{
    
    
}

/* ------------- Funcoes globais -------------*/

/* Retorna verdadeiro caso esteja "pescando" e atualiza alguns parametros */
bool isMouseFishing(gameManager_t *gm)
{
    /* checa toda matriz para ver se o mouseX e mouseY estao em algum fishX fishY */
    int i, j;

    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            if (gm->mouseX >= gm->matrix[i][j]->fishX && gm->mouseX <= gm->matrix[i][j]->fishX + 50 &&
                gm->mouseY >= gm->matrix[i][j]->fishY && gm->mouseY <= gm->matrix[i][j]->fishY + 50){ /* ve se o clique esta no peixe atual */
                    if ((!gm->matrix[i][j]->selected) && (gm->selected == 0)){ /* eh o primeiro a ser selecionado */
                        gm->matrix[i][j]->selected = true;
                        gm->selectedFishes[gm->selected] = gm->matrix[i][j];
                        gm->selected++;
                        return true;
                    } else if (gm->selected == 1){ /* ja ta clicado */
                        if (gm->matrix[i][j]->selected){ /* ja foi selecionado, des-seleciona */
                            gm->matrix[i][j]->selected = 0;
                            gm->selected--;
                            return true;
                        } else if (isNeighbor(gm, gm->matrix[i][j])){ /* verifica se eh um vizinho */
                            gm->matrix[i][j]->selected = 1;
                            gm->selectedFishes[gm->selected] = gm->matrix[i][j];
                            gm->selected++;
                            return true;
                        } else { /* nao eh um vizinho */
                            gm->selectedFishes[gm->selected - 1]->selected = 0;
                            gm->selected--;
                            return true;
                        }
                    }
            }
        }
    }

    return false;
}

/* Atualiza a LOGICA */
void updateLogic(gameManager_t *gm)
{   
    bool done = false;

    while (! done)
    {
        switch (gm->logicState){
            /* JOGO EM ESPERA */
            case STANDBY:
                /* nao faz nada em espera */
                done = true;
                break;

            /* ANIMACAO DE DUAS PECAS TROCANDO DE LUGAR */
            case SWITCHING:
                al_rest(0.2);
                switchFishes(gm);
                gm->logicState = PROCESSING;
                done = true;
                break;
            
            /* VERIFICA EM BUSCA DE MATCH */
            case PROCESSING:
                checkAndRemoveMatches(gm);
                fillBlank(gm);
                gm->logicState = STANDBY;
                done = true;
                break;

            /* JANELA DE AJUDA */
            case HELP_WD:
                done = true;
                break;
        }
    }
}