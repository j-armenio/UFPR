#include <stdio.h>
#include <stdlib.h>

#include "libbinary.h"
#include "libutil.h"

void copyBinary(FILE *source, int fileSize, FILE *target);
void pasteMember(FILE *bkp, int copyStart, int size, int writeStart);
void shiftRight(FILE *file, int bStart, int bEnd, int writeEnd);
void shiftLeft(FILE *file, int bStart, int bEnd, int writeStart);
directory *readBackupToDirectory(FILE *bkp);
void extractContent(FILE *bkp, directory *dirList);

// Copia o conteúdo binário de um arquivo para outro usando um buffer de 1024 bytes
void copyBinary(FILE *source, int fileSize, FILE *target)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int iterations = fileSize / 1024;
    int rest = fileSize % 1024;
    int i;
    for (i = 0; i < iterations; i++) {
        fread(buffer, sizeof(char), 1024, source);
        fwrite(buffer, sizeof(char), 1024, target);
    }
    fread(buffer, sizeof(char), rest, source);
    fwrite(buffer, sizeof(char), rest, target);

    free(buffer);
}

// Copia o conteudo binário de um bloco e escreve em outra parte do arquivo
void pasteMember(FILE *bkp, int copyStart, int size, int writeStart)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int bCopy = copyStart;
    int bPaste = writeStart;

    int iterations = (size / 1024);
    int rest = size % 1024;

    for (int i = 0; i < iterations; i++) {
        fseek(bkp, bCopy, SEEK_SET);
        fread(buffer, 1, 1024, bkp);
        bCopy += 1024;
        fseek(bkp, bPaste, SEEK_SET);
        fwrite(buffer, 1, 1024, bkp);
        bPaste += 1024;
    }
    fseek(bkp, bCopy, SEEK_SET);
    fread(buffer, 1, rest, bkp);
    fseek(bkp, bPaste, SEEK_SET);
    fwrite(buffer, 1, rest, bkp);

    free(buffer);
}

// Faz o shift (deslocamento) de um bloco para a direita
void shiftRight(FILE *file, int bStart, int bEnd, int writeEnd)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int blockSize = bEnd - bStart;
    int iterations = blockSize / 1024;
    int rest = blockSize % 1024;

    for (int i = 0; i < iterations; i++) {
        fseek(file, bEnd - 1024, SEEK_SET);
        fread(buffer, 1, 1024, file);
        bEnd -= 1024;
        fseek(file, writeEnd - 1024, SEEK_SET);
        fwrite(buffer, 1, 1024, file);
        writeEnd -= 1024;
    }
    fseek(file, bEnd - rest, SEEK_SET);
    fread(buffer, 1, rest, file);
    fseek(file, writeEnd - rest, SEEK_SET);
    fwrite(buffer, 1, rest, file);

    free(buffer);
}

// Faz o shift (deslocamento) de um bloco para a Esquerda
void shiftLeft(FILE *file, int bStart, int bEnd, int writeStart)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int blockSize = bEnd - bStart;
    int iterations = blockSize / 1024;
    int rest = blockSize % 1024;

    for (int i = 0; i < iterations; i++) {
        fseek(file, bStart, SEEK_SET);
        fread(buffer, 1, 1024, file);
        bStart += 1024;
        fseek(file, writeStart, SEEK_SET);
        fwrite(buffer, 1, 1024, file);
        writeStart += 1024;
    }
    fseek(file, bStart, SEEK_SET);
    fread(buffer, 1, rest, file);
    fseek(file, writeStart, SEEK_SET);
    fwrite(buffer, 1, rest, file);

    free(buffer);
}

// Preenche uma lista directory com as informações de um arquivo bkp 
directory *readBackupToDirectory(FILE *bkp)
{
    int binarySize;
    fread(&binarySize, sizeof(int), 1, bkp);

    directory *dir = createDirectory();
    if (! dir) {
        printf("Erro ao alocar memória para o diretório.\n");
        return NULL;
    }

    int memCount;

    fseek(bkp, 4, SEEK_SET); // Pula o do int no começo do arquivo
    fseek(bkp, binarySize, SEEK_CUR); // Pula todo conteudo binario
    fread(&memCount, 1, sizeof(int), bkp); // Le a quantidade de membros

    char *strBuffer;
    int intBuffer;
    unsigned long unLongBuffer;
    size_t tBuffer;

    int i;
    for (i = 0; i < memCount; i++)
    {
        member *newMember = NULL;
        newMember = (member *)malloc(sizeof(member));
        if (! newMember) {
            printf("Erro ao alocar memória para o membro.\n");
            return NULL;
        };

        strBuffer = (char *)malloc(PATH_MAX_SIZE);
        fread(strBuffer, 1, PATH_MAX_SIZE, bkp);
        memcpy(newMember->path, strBuffer, PATH_MAX_SIZE);
        free(strBuffer);

        strBuffer = (char *)malloc(NAME_MAX_SIZE);
        fread(strBuffer, 1, NAME_MAX_SIZE, bkp);
        memcpy(newMember->name, strBuffer, NAME_MAX_SIZE);
        free(strBuffer);

        strBuffer = (char *)malloc(25);
        fread(&tBuffer, 1, sizeof(time_t), bkp);
        newMember->modificationDate = tBuffer;
        free(strBuffer);

        fread(&intBuffer, 1, sizeof(int), bkp);
        newMember->size = intBuffer;

        fread(&intBuffer, 1, sizeof(int), bkp);
        newMember->uid = intBuffer;

        fread(&intBuffer, 1, sizeof(int), bkp);
        newMember->permissions = intBuffer;

        fread(&intBuffer, 1, sizeof(int), bkp);
        newMember->positionList = intBuffer;

        fread(&unLongBuffer, 1, sizeof(unsigned long), bkp);
        newMember->positionBkp = unLongBuffer;

        insertMemberToDir(dir, newMember);
    }

    return dir;
}

// Extrai conteudo de um bkp para uma pasta
void extractContent(FILE *bkp, directory *dirList)
{
    member *currentMember = dirList->head;

    fseek(bkp, 0, SEEK_SET);
    while (currentMember != NULL)
    {
        createDirectories(currentMember->path);

        FILE *file = fopen(currentMember->path, "wb+");
        if (! file) {
            printf("Erro ao abrir arquivo %s.\n", currentMember->path);
            return;
        }

        fseek(bkp, currentMember->positionBkp, SEEK_SET);

        copyBinary(bkp, currentMember->size, file);
        
        fclose(file);
        currentMember = currentMember->next;
    }
}