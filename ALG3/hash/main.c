#include <stdio.h>
#include "hasht.h"

int main()
{
    HtItem *T1 = createTable();
    HtItem *T2 = createTable();

    char command;
    int value;

    fscanf(stdin, "%c %d", &command, &value);
    getc(stdin);
    
    // Continua realizando leituras enquanto houver entradas disponíveis
    while (! feof(stdin)){
        if (command == 'i')
            insertHt(T1, T2, value);
        else if (command == 'r')
            removeHt(T1, T2, value);
    
        fscanf(stdin, "%c %d", &command, &value);
        getc(stdin);
    }

    printTables(T1, T2);

    freeTables(T1, T2);
}