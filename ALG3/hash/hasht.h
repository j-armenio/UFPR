#include <stdlib.h>

typedef struct HtItem {
    int table;
    int data;
    int index;
} HtItem;

HtItem *createTable();
void freeTables(HtItem *T1, HtItem *T2);
int insertHt(HtItem *T1, HtItem *T2, int key);
int removeHt(HtItem *T1, HtItem *T2, int key);
void printTables(HtItem *T1, HtItem *T2);