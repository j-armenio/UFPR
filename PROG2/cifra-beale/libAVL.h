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

node_t *inclusion(node_t *root, int key, char letter);
node_t *binarySearch(node_t *root, int key);
void inOrder(node_t *root);
void destroyTree(node_t *root);

#endif