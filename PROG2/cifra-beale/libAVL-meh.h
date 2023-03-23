#ifndef _LIB_AVL_H_
#define _LIB_AVL_H_

#include <stdio.h>

typedef struct node {
    int key;
    char letter;
    struct node *left;
    struct node *right;
    int height;
} node_t;

node_t *insertNode(node_t* root, int key, char letter);
void inOrderTraversal(node_t *root);
int nodeHeight(node_t *root);
int balanceFactor(node_t *root);
node_t *rotateRight(node_t *root);
node_t *rotateLeft(node_t *root);
node_t *LL(node_t *root);
node_t *RR(node_t *root);
node_t *LR(node_t *root);
node_t *RL(node_t *root);

#endif