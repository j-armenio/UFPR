#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libmember.h"
#include "libutil.h"
#include "libbinary.h"
#include "libmemUtil.h"

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
    char *pointerAux = pathCopy;
    strcpy(pathCopy, path);
    pathCopy = getRelativePath(pathCopy);
    strcpy(newMember->path, pathCopy);
    free(pointerAux);

    newMember->modificationDate = fileInfo.st_mtime;

    newMember->size = fileInfo.st_size;
    newMember->uid = fileInfo.st_uid;
    newMember->permissions = fileInfo.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

    newMember->positionList = -1;
    newMember->positionBkp = -1;

    newMember->next = NULL;
    
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

directory *insertMemberToDir(directory *dir, member *newMember)
{
    if (dir == NULL || newMember == NULL) {
        printf("Erro ao inserir membro no diretório.\n");
        return NULL;
    }

    /* Lista com elementos */
    if (dir->head != NULL) {
        dir->tail->next = newMember;
        dir->tail = newMember;
        newMember->positionList = dir->memberCount;
    }

    /* Lista vazia */
    if (dir->head == NULL) {
        dir->head = newMember;
        dir->tail = newMember;
        newMember->positionList = 0;
    }

    dir->memberCount++;
    newMember->next = NULL;

    return dir;
}

void removeMemberFromDir(directory *dir, member *m)
{
    member *currentMember = dir->head;
    member *previousMember = NULL;

    while (currentMember != NULL) {
        if (currentMember == m) {
            if (previousMember == NULL) {
                dir->head = currentMember->next;
            } else {
                previousMember->next = currentMember->next;
            }

            if (currentMember == dir->tail)
                dir->tail = previousMember;

            free(currentMember);
            dir->memberCount--;
            updateListPositions(dir);
            return;
        }

        previousMember = currentMember;
        currentMember = currentMember->next;
    }
}

// Move o m2 para atras do m1 e atualiza todos os ponteiros
void shiftMoveInList(directory *lst, member *target, member *move)
{
    if (target->next == move)
        return;

    if (target->positionList < move->positionList) {
        member *aux1 = target->next; // proximo do target
        member *aux2 = move->next;   // proximo do move
        member *aux3 = target;       // anterior ao move
        while (aux3->next != move)
            aux3 = aux3->next;

        target->next = move;
        move->next = aux1;
        aux3->next = aux2;
    } else if (target->positionList > move->positionList) {
        member *aux1 = target->next; // proximo do target
        member *aux2 = move->next;   // proximo do move
        member *aux3 = lst->head;    // anterior ao move
        if (aux3 == move) {
            target->next = move;
            move->next = aux1;
            lst->head = aux2;
        } else {
            while (aux3->next != move)
                aux3 = aux3->next;

            target->next = move;
            move->next = aux1;
            aux3->next = aux2;
        }
    } 

    // Atualiza tail
    member *aux = lst->head;
    while (aux->next != NULL)
        aux = aux->next;
    lst->tail = aux;
    lst->tail->next = NULL;

    updateListPositions(lst);
    updateBkpPositions(lst);
}
