#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libtest.h"
#include "libgeneral.h"
#include "liblist.h"

/* void testAvl()
{
    int choice, i, go;
    node_t *root = NULL;
    char letter;

    printf("Testando a AVL...\n");

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

    srand ( time(NULL) );

    for (i = 0; i < 10000; i++)
    {
        letter = randomNum(97, 122);
        root = AVL_inclusion(root, i, letter);
    }

    AVL_inOrder(root);

} */

void testList(){
    int go, choice, i;
    char letter;

    listLetters_t *list = NULL;
    list = createLettersList();

    printf("Testando a Lista...\n");

    // TESTE 1: ESCOLHE MANUALMENTE QUAIS COLOCAR

    i = 0;

    while (go)
    {
        printf("1. Insert\n");
        printf("2. Print List\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter the letter to be inserted: ");
                scanf(" %c", &letter);
                if (! searchLetter(letter, list)){
                    insertLetter(letter, list);
                    insertPosition(i, list->tail);
                } else if (searchLetter(letter, list)){
                    // insere posicao e aumenta repeticoes
                    insertPosition(i, searchLetter(letter, list));
                    searchLetter(letter, list)->repetitions++;                    
                }
                i++;
                break;
            case 2:
                printf("Print List:\n");
                printList(list);
                printf("\n");
                break;
            case 3:
                go = 0;
                break;
            default:
                printf("Invalid choice\n");
        }       
    }
}

void testIndex()
{
    listLetters_t *list = NULL;

    list = createLettersList();

    list = indexAllList(list);

    printList(list);
}