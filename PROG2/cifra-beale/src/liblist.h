#ifndef _LIB_LIST_
#define _LIB_LIST_

typedef struct nodePosition {
    int position;
    struct nodePosition *next;
} nodePosition_t;

typedef struct nodeLetter {
    char letter;
    int repetitions;
    struct nodeLetter *next;
    struct nodePosition *positions;
} nodeLetter_t;

typedef struct listLetters {
    int size;
    nodeLetter_t *head;
    nodeLetter_t *tail;
} listLetters_t;

listLetters_t *createLettersList();
nodeLetter_t *insertLetter(char letter, listLetters_t *list);
nodePosition_t *insertPosition(int position, nodeLetter_t *node);
nodeLetter_t *searchLetter(char letter, listLetters_t *list);
nodeLetter_t *searchPosition(int position, listLetters_t *list);
void printList(listLetters_t *list);
int printListToFile(listLetters_t *list, char *filePath);
listLetters_t *indexAllList(listLetters_t *list);

#endif