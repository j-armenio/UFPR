#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ./testExec 1 bkp.vpp file1 file2

int testAllocation(void *pointer)
{
    if (pointer == NULL) {
        printf("Erro ao alocar.\n");
        exit(1);
    }
    return 0;
}

int main(int argc, char **argv)
{
    FILE *backup = NULL;
    FILE *file = NULL;
    unsigned int fileSize;

    if (! strcmp(argv[1], "1"))
    {
        // abre bkp.vpp
        backup = fopen(argv[2], "wb");
        testAllocation(backup);

        // INSERE NO BACKUP
        for (int i = 3; i < argc; i++) { 
            file = fopen(argv[i], "rb");
            testAllocation(file);

            fileSize = 0;

            fseek(file, 0, SEEK_END);
            fileSize = ftell(file);
            printf("%d\n", fileSize);
            fseek(file, 0, SEEK_SET);
            fwrite(&fileSize, sizeof(int), 1, backup);

            char *buffer;
            buffer = (char *)malloc(sizeof(char) * fileSize);
            fread(buffer, fileSize, 1, file);
            fwrite(buffer, fileSize, 1, backup);

            fclose(file);
        }
        fclose(backup);
    } else {
        // REMOVE DO BACKUP
        backup = fopen(argv[2], "rb");
        testAllocation(backup);

        file = fopen("extract.jpeg", "wb");
        testAllocation(file);

        // size mp3 13743
        fileSize = 13743;

        char *buffer;
        buffer = (char *)malloc(sizeof(char) * fileSize);
        fread(buffer, fileSize, 1, backup);
        fwrite(buffer, fileSize, 1, file);
        
        fclose(file);
        fclose(backup);
        return 0;
    }
}