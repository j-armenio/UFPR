#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isLetterOrNumber(int c)
{
    if (((c >= 48) && (c <= 57)) || ((c >= 97) && (c <= 122)))
        return 1;
    else
        return 0;
}


int main()
{
    int position, c, pc;

    FILE *file = fopen("text.txt", "r");
    if (! file){
        printf("Failed to open text.txt.\n");
        return 1;
    }

    while ((c = fgetc(file)) != EOF){
        if (position == 0){
            if (isLetterOrNumber(c)){
                printf("%c", c);
                position++;
            }
            break;
        }

        pc = fseek(file, -1, SEEK_CUR);
        c = tolower(c);
        pc = tolower(pc);

        if (isLetterOrNumber(c)){
            if (isLetterOrNumber(pc)){
                printf("%c %c", c, pc);
                position++;
           }
        }
    }
}