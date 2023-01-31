#include <stdlib.h>

typedef struct HtItem {
    char *table;
    int data;
} HtItem;

HtItem *createTable();
void freeTables(HtItem *T1, HtItem *T2);
int insertHt(HtItem *T1, HtItem *T2, int key);
int removeHt(HtItem *T1, HtItem *T2, int key);
void printTables(HtItem *T1, HtItem *T2);