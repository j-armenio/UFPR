#include <stdio.h>
#include <stdlib.h>

#include "libavl.h"

// Retorna um ponteiro para um nodevo node
node_t *createNode(int key, char letter)
{
    node_t *node = malloc(sizeof(node_t));
    if (! node){
        perror("Falha ao alocar Nó\n");
        return NULL;
    }

    node->key = key;
    node->letter = letter;
    node->height = 0;
    node->right = NULL;
    node->left = NULL;

    return node;
}

// Busca uma key na arvore e caso ele exista, retorna seu node
node_t *AVL_binarySearch(node_t *root, int key)
{
    if (root == NULL)
        return NULL;

    if (root->key == key)
        return root;

    if (key < root->key)
        return AVL_binarySearch(root->left, key);
    else
        return AVL_binarySearch(root->right, key);
}

// Calcula a height de qualquer node da arvore
int heightNode(node_t *node)
{
    int altLeft = 0;
    int altRight = 0;

    if (node == NULL)
        return -1;

    altLeft = heightNode(node->left);
    altRight = heightNode(node->right);


    if (altLeft > altRight)
        return altLeft + 1;
    else
        return altRight + 1;
}

// Calcula a profundidade de qualquer node da arvore, por travessia pre-ordem
void depthNode(node_t *node, int depth)
{
    if (node == NULL)
        return;

    node->depth = depth; 
    depth++;
    depthNode(node->left, depth);
    depthNode(node->right, depth);
}

// Calcula o Fator de Balanceamento de um node qualquer da arvore
int balanceFactor(node_t *node)
{
    if (node->left == NULL && node->right == NULL)
        return 0;

    return heightNode(node->left) - heightNode(node->right);}

// Atualiza a altura de todos nodes da arvore
void heightUpdate(node_t *root)
{
    if (root == NULL)
        return;
    
    heightUpdate(root->left);
    root->height = heightNode(root);
    heightUpdate(root->right);
}

// Realiza a rotação para esquer e retorna a nova raiz
node_t *rotLeft(node_t *x)
{
    node_t *y = x->right;
    node_t* T2 = y->left;
  
    // Faz a rotação
    y->left = x;
    x->right = T2;
  
    // Atualiza alturas
    heightUpdate(x);
    heightUpdate(y);
  
    // Retorna a nova raiz
    return y;
}

// Realiza a rotação para righteita e retorna a nodeva root
node_t *rotRight(node_t *y)
{
    node_t *x = y->left;
    node_t *T2 = x->right;
  
    // Executa rotação
    x->right = y;
    y->left = T2;
  
    // Atualiza altura da arvore
    heightUpdate(y);
    heightUpdate(x);

    return x;
}

node_t *balanceAVL(node_t* root, int key)
{
    node_t* nodeBal;
    if(balanceFactor(root) > 1){
        if(balanceFactor(root->left) < 0)
            root->left = rotLeft(root->left);
        nodeBal = rotRight(root);
    }
    else if(balanceFactor(root) < -1){
        if(balanceFactor(root->right) > 0)
            root->right = rotRight(root->right);
        nodeBal = rotLeft(root);
    }

    heightUpdate(root);

    return nodeBal;
}

// Inclui um nodedo com a key inserida e retorna a root
node_t *AVL_inclusion(node_t *root, int key, char letter)
{
    node_t* nodeBal;

    // Critério de parada: AVL vazia ou chegou a NULL
    if (root == NULL){
        root = createNode(key, letter);
        return root;
    }
    // Encontra a posição da key na árvore e adiciona o nodedo
    if (key > root->key){
        root->right = AVL_inclusion(root->right, key, letter);
    }
    else if (key < root->key){
        root->left = AVL_inclusion(root->left, key, letter);
    } 
    else {
        return root;
    }

    heightUpdate(root);

    root = balanceAVL(root, key);

    // retorna a raiz (não alterada)
    return root;
}

// Travesia EmOrdem(left->root->right)
void AVL_inOrder(node_t *root)
{
    if (root == NULL)
        return;
    
    AVL_inOrder(root->left);
    printf("|%c|%d|\n", root->letter, root->key);
    AVL_inOrder(root->right);
}

// Libera a memoria de toda arvore
void AVL_destroyTree(node_t *root)
{
    if (root == NULL)
        return;
    AVL_destroyTree(root->left);
    AVL_destroyTree(root->right);
    free(root);
}