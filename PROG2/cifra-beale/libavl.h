#ifndef _LIB_AVL_TREE_
#define _LIB_AVL_TREE_

typedef struct node {
    int key;
    int height;
    int depth;
    char letter;
    struct node *parent;
    struct node *left;
    struct node *right;
} node_t;

node_t *AVL_inclusion(node_t *root, int key, char letter);
node_t *AVL_binarySearch(node_t *root, int key);
void AVL_inOrder(node_t *root);
void AVL_destroyTree(node_t *root);

#endif