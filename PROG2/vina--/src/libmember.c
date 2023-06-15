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

    strftime(newMember->modificationDate, 30, "%c", localtime(&fileInfo.st_mtime));

    newMember->size = fileInfo.st_size;
    newMember->uid = fileInfo.st_uid;
    newMember->permissions = fileInfo.st_mode;

    newMember->position = 0;

    return newMember;
}

void printMember(member *member)
{
    printf("Nome: %s\n", member->name);
    printf("Caminho: %s\n", member->path);
    printf("Data de modificação: %s\n", member->modificationDate);
    printf("Tamanho: %d\n", member->size);
    printf("UID: %d\n", member->uid);
    printf("Permissões: %d\n", member->permissions);
    printf("Posição: %d\n", member->position);
}

/* --- Externas --- */

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