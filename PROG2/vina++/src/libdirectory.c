#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "libdirectory.h"

#define STRING_MAX_SIZE 256

member_t *allocateEmptyMember()
{
    member_t *newMember = NULL;

    newMember = malloc(sizeof(member_t));
    if (! newMember){
        fprintf(stderr, "Erro ao alocar novo membro.\n");
        exit(1);
    }

    newMember->name = NULL;
    newMember->name = malloc(sizeof(char) * STRING_MAX_SIZE);
    if (! newMember->name){
        fprintf(stderr, "Erro ao alocar nome do membro.\n");
        exit(1);
    }

    newMember->location = NULL;
    newMember->location = malloc(sizeof(char) * STRING_MAX_SIZE);
    if (! newMember->location){
        fprintf(stderr, "Erro ao alocar localização do membro.\n");
        exit(1);
    }

    newMember->modificationDate = 0;
    newMember->uid = 0;
    newMember->permissions = 0;
    newMember->size = 0;
    newMember->order = 0;

    return newMember;
}

member_t *fillMemberInfo(char *currentPath)
{
    member_t *newMember = NULL;
    struct stat fileInfo;

    newMember = allocateEmptyMember();

    if (stat(currentPath, &fileInfo) == -1){
        fprintf(stderr, "Erro ao obter informações do arquivo.\n");
        exit(1);
    }

    char *fileName = strrchr(currentPath, '/');
    if (fileName != NULL)
        fileName++;
    else
        fileName = newMember->name;

    printf("Nome do arquivo: %s\n", newMember->name);

    newMember->modificationDate = fileInfo.st_mtime;

    printf("Data de modificação: %ld\n", newMember->modificationDate);

    return newMember;
}

void addMember(directory_t *directory, member_t *newMember)
{
    
}

void removeMember(directory_t *directory, member_t *targetedMember)
{

}