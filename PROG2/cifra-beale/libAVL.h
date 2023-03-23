#ifndef _LIB_AVL_TREE_
#define _LIB_AVL_TREE_

typedef struct node {
    int key;
    char letter;
    int height;
    struct node *left;
    struct node *right;
} node_t;

node_t *insertNode(node_t *root, int key, char letter);
void inOrderTraversal(node_t *root);
void destroyTree(node_t *root);

#endif