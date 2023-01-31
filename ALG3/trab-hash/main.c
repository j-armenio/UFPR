#include <stdio.h>
#include "myht.h"

int main()
{
    HtItem *T1 = createTable();
    HtItem *T2 = createTable();

    insertHt(T1, T2, 10);
    insertHt(T1, T2, 22);
    insertHt(T1, T2, 4);
    insertHt(T1, T2, 15);
    insertHt(T1, T2, 59);
    removeHt(T1, T2, 15);
    removeHt(T1, T2, 22);
    removeHt(T1, T2, 59);

    printTables(T1, T2);

    freeTables(T1, T2);
}