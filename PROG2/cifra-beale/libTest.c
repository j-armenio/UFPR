#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libTest.h"
#include "libAVL.h"

void testAvl()
{
    int choice, i;
    node_t *root = NULL;
    char letter;

    i = 0;

    printf("Testando a AVL...\n");

    while (1)
    {
        printf("1. Insert\n");
        printf("2. Print Inorder\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter the letter to be inserted: ");
                scanf(" %c", &letter);
                root = insertNode(root, i, letter);
                i++;
                break;
            case 2:
                printf("Inorder Traversal: ");
                inOrderTraversal(root);
                printf("\n");
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice\n");
        }       
    }
}