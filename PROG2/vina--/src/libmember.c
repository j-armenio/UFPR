#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libmember.h"
#include "libutil.h"

/* --- Prototipos --- */

/* --- Internas --- */

void printMember(member *member)
{
    printf("Nome: %s\n", member->name);
    printf("Caminho: %s\n", member->path);
    printf("Data de modificação: %ld\n", member->modificationDate);
    printf("Tamanho: %d\n", member->size);
    printf("UID: %d\n", member->uid);
    printf("Permissões: %d\n", member->permissions);
    printf("Posição: %d\n", member->position);
}

/* --- Externas --- */

member *createMember(char *path)
{
    member *newMember = NULL;
    struct stat fileInfo;

    newMember = (member *)malloc(sizeof(member));
    if (! newMember) {
        printf("Erro ao alocar memória para o membro.\n");
        return NULL; 
    }

    if (stat(path, &fileInfo) == -1) {
        printf("Erro ao obter informações do arquivo.\n");
        return NULL;
    }

    char *fileName = strrchr(path, '/');
    if (fileName != NULL)
        fileName++;
    else
        fileName = path;
    strcpy(newMember->name, fileName);

    char *pathCopy = (char *)malloc(strlen(path) + 1);
    strcpy(pathCopy, path);
    pathCopy = getRelativePath(pathCopy);
    strcpy(newMember->path, pathCopy);
    free(pathCopy);

    newMember->modificationDate = fileInfo.st_mtime;

    newMember->size = fileInfo.st_size;
    newMember->uid = fileInfo.st_uid;
    newMember->permissions = fileInfo.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

    newMember->position = -1;

    newMember->next = NULL;
    newMember->previous = NULL;

    return newMember;
}

directory *createDirectory()
{
    directory *newDir = NULL;
    newDir = (directory *)malloc(sizeof(directory));
    if (! newDir) {
        printf("Erro ao alocar memória para o diretório.\n");
        return NULL;
    }

    newDir->head = NULL;
    newDir->tail = NULL;
    newDir->memberCount = 0;

    return newDir;
}

void freeDirectory(directory *dir)
{
    member *currentMember = dir->head;
    member *nextMember = NULL;

    while (currentMember != NULL) {
        nextMember = currentMember->next;
        free(currentMember);
        currentMember = nextMember;
    }

    free(dir);
}

void printDirectory(directory *dir)
{
    member *currentMember = dir->head;

    while (currentMember != NULL) {
        printMember(currentMember);
        currentMember = currentMember->next;
    }
}

directory *insertMemberToDir(directory *dir, member *newMember)
{
    if (dir == NULL || newMember == NULL) {
        printf("Erro ao inserir membro no diretório.\n");
        return NULL;
    }

    /* Lista com elementos */
    if (dir->head != NULL) {
        dir->tail->next = newMember;
        newMember->previous = dir->tail;
        dir->tail = newMember;
        newMember->position = dir->tail->previous->position + 1;
    }

    /* Lista vazia */
    if (dir->head == NULL) {
        dir->head = newMember;
        dir->tail = newMember;
        newMember->position = 0;
    }

    dir->memberCount++;
    newMember->next = NULL;

    return dir;
}

int getFilesTotalSize(directory *dir)
{
    member *currentMember = dir->head;
    int totalSize = 0;

    while (currentMember != NULL) {
        totalSize += currentMember->size;
        currentMember = currentMember->next;
    }

    return totalSize;
}

member *getMemberByPosition(directory *dir, int position)
{
    member *currentMember = dir->head;

    while (currentMember != NULL) {
        if (currentMember->position == position)
            return currentMember;
        currentMember = currentMember->next;
    }

    return NULL;
}

void writeMember(FILE *bkp, member *m)
{
    fwrite(m->path, 1, PATH_MAX_SIZE, bkp);
    fwrite(m->name, 1, NAME_MAX_SIZE, bkp);
    fwrite(&m->modificationDate, 1, sizeof(time_t), bkp);
    fwrite(&m->size, 1, sizeof(int), bkp);
    fwrite(&m->uid, 1, sizeof(int), bkp);
    fwrite(&m->permissions, 1, sizeof(int), bkp);
    fwrite(&m->position, 1, sizeof(int), bkp);
}

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
        newMember->position = intBuffer;

        insertMemberToDir(dir, newMember);
    }

    return dir;
}

void extractToDir(char *dirName, FILE *bkp, directory *dirList)
{
    char *buffer = (char *) malloc(sizeof(char) * 1024);
    if (! buffer) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    // Copia o conteudo do arquivo para os files (com buffer de 1024 bytes)
    member *currentMember = dirList->head;
    while (currentMember != NULL) {
        char *newPath = (char *) malloc(sizeof(char) * (strlen(dirName) + strlen(currentMember->name) + 2));
        if (! newPath) {
            printf("Erro ao alocar memória para o novo caminho.\n");
            return;
        }

        strcpy(newPath, dirName);
        strcat(newPath, "/");
        strcat(newPath, currentMember->name);

        FILE *file = fopen(currentMember->path, "wb");
        if (! file) {
            printf("Erro ao abrir arquivo %s.\n", currentMember->path);
            return;
        }

        int bytesLeft = currentMember->size;
        while (bytesLeft > 0) {
            int bytesToRead = bytesLeft > 1024 ? 1024 : bytesLeft;
            fread(buffer, 1, bytesToRead, bkp);
            fwrite(buffer, 1, bytesToRead, file);
            bytesLeft -= bytesToRead;
        }

        free(newPath);
        fclose(file);
        currentMember = currentMember->next;
    }
}