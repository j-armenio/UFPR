#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libAVL.h"

// Insere um nodo na árvore do nodo raiz referenciado
node_t *insertNode(node_t* root, int key, char letter)
{
    if (root == NULL){
        root = (node_t *)malloc(sizeof(node_t));
        root->key = key;
        root->letter = letter;
        root->right = NULL;
        root->left = NULL;
    } else if (key < root->key) {
        root->left = insertNode(root->left, key, letter);
        if (balanceFactor(root) == 2)
            if (key < root->left->key)
                root = LL(root);
            else
                root = LR(root);
    } else if (key > root->key) {
        root->left = insertNode(root->left, key, letter);
        if (balanceFactor(root) == -2)
            if (key > root->right->key)
                root = RR(root);
            else
                root = RL(root);
    } else
        printf("Chave duplicada.\n");
    
    return root;
}

// Faz a travesia em ordem imprimindo as chaves e letras em cada nó
void inOrderTraversal(node_t *root)
{
    if (root == NULL){
        return;
    } else {
        inOrderTraversal(root->left);
        printf("|%d|%c| ", root->key, root->letter);
        inOrderTraversal(root->right);
    }
}

int nodeHeight(node_t *root)
{
    int lh, rh;

    if (root == NULL)
        return 0;

    if (root->left == NULL)
        lh = 0;
    else
        lh = 1 + nodeHeight(root->left);
    
    if (root->right == NULL)
        rh = 0;
    else
        rh = 1 + nodeHeight(root->right);
    
    if (lh > rh)
        return lh;
    else
        return rh;
}

int balanceFactor(node_t *root)
{
    int lh, rh;

    if (root == NULL)
        return 0;
    
    if (root->left == NULL)
        lh = 0;
    else
        lh = 1 + nodeHeight(root->left);
    
    if (root->right == NULL)
        rh = 0;
    else
        rh = 1 + nodeHeight(root->right);
    
    return (lh - rh);
}

node_t *rotateLeft(node_t *root)
{
	node_t *temp;
	temp = root->left;
	free(root->left);
	temp->right=root;
	root->height = nodeHeight(root);
	temp->height = nodeHeight(temp);

	return temp;
}

node_t *rotateRight(node_t *root)
{
	node_t *temp;
	temp = root->right;
	free(root->right);   // root->right=NULL;
	temp->left = root;
	root->height = nodeHeight(root);
	temp->height = nodeHeight(temp);

	return temp;
}

node_t *LL(node_t *root)
{
	root = rotateLeft(root);
	return root;
}

node_t *RR(node_t *root)
{
	root = rotateRight(root);
	return root;
}

node_t *LR(node_t *root)
{
	root = rotateRight(root->left);  //root->left=rotateRight(root->left);
	//root=rotateRight(root);
	return root;
}

node_t *RL(node_t *root)
{
	root = rotateLeft(root->right);  //root->right=rotateRight(root->right);
	//root=rotateLeft(root);
	return root;
}