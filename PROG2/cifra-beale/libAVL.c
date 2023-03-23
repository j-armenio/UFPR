#include <stdio.h>
#include <stdlib.h>

#include "libAVL.h"

node_t *insertNode(int key, char letter)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (!node){
        perror("Erro ao alocar memória para o nodo.\n");
        return NULL;
    }

    node->key = key;
    node->letter = letter;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;

    return node;
}

node_t *binarySearch()
{
    
}