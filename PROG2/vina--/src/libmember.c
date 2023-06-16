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

    newMember->position = -1;

    newMember->next = NULL;
    newMember->previous = NULL;

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

directory *insertMemberToDir(directory *dir, char *path)
{
    member *newMember = NULL;
    newMember = createMember(path);

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
    char quotationMarks = '\"';
    char comma = ',';

    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(m->name, 1, strlen(m->name), bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&comma, 1, 1, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(m->path, 1, strlen(m->path), bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&comma, 1, 1, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(m->modificationDate, 1, 25, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&comma, 1, 1, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&m->size, 1, sizeof(int), bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&comma, 1, 1, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&m->uid, 1, sizeof(int), bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&comma, 1, 1, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&m->permissions, 1, sizeof(int), bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&comma, 1, 1, bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite(&m->position, 1, sizeof(int), bkp);
    fwrite(&quotationMarks, 1, 1, bkp);
    fwrite("\n", 1, 1, bkp);
}