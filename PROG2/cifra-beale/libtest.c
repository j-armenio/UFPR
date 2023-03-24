#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libtest.h"
#include "libavl.h"
#include "libgeneral.h"

void testAvl()
{
    int choice, i, go;
    node_t *root = NULL;
    char letter;

    printf("Testando a AVL...\n");

    // TESTE 1: ESCOLHE MANUALMENTE QUAIS COLOCAR
    i = 0;
    go = 1;

    while (go)
    {
        printf("1. Insert\n");
        printf("2. Print Inorder\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter the key to be inserted: ");
                scanf(" %c", &letter);
                root = AVL_inclusion(root, i, letter);
                i++;
                break;
            case 2:
                printf("Inorder Traversal:\n");
                AVL_inOrder(root);
                printf("\n");
                break;
            case 3:
                go = 0;
                break;
            default:
                printf("Invalid choice\n");
        }       
    }

    // TESTE 2: COLOCA UM MONTE
    srand ( time(NULL) );

    for (i = 0; i < 10000; i++)
    {
        letter = randomNum(97, 122);
        root = AVL_inclusion(root, i, letter);
    }

    AVL_inOrder(root);

}