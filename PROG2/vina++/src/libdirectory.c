#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "libdirectory.h"
#include "libutil.h"

#define STRING_MAX_SIZE 256

/* ------------------ PROTOTYPES ------------------ */

void printMember(member_t *member);
member_t *allocateEmptyMember();
member_t *createNewMember(char *filePath);
void freeMember(member_t *member);
directory_t *createDirectory();
directory_t *addMemberToDirectory(directory_t *directory, char *filePath);
void removeMemberFromDirectory(directory_t *directory, member_t *targetedMember);

/* ------------------ INTERNAL FUNCTIONS ------------------ */

void printMember(member_t *member)
{
    printf("Nome: %s\n", member->name);
    printf("Localização: %s\n", member->location);
    printf("Data de modificação: %s", member->modificationDate);
    printf("UID: %d\n", member->uid);
    printf("Permissões: %d\n", member->permissions);
    printf("Tamanho: %d\n", member->size);
    printf("Ordem: %d\n", member->order);

    return;
}

member_t *allocateEmptyMember()
{
    member_t *newMember = NULL;

    newMember = malloc(sizeof(member_t));
    if (! newMember){
        fprintf(stderr, "Erro ao alocar novo membro.\n"); // PERGUNTAR SE EH MELHOR PRINTF OU FPRINTF
        exit(1);
    }

    newMember->name = NULL;
    newMember->name = malloc(sizeof(char) * STRING_MAX_SIZE);
    if (! newMember->name){
        printf("Erro ao alocar nome do membro.\n");
        exit(1);
    }

    newMember->location = NULL;
    newMember->location = malloc(sizeof(char) * STRING_MAX_SIZE);
    if (! newMember->location){
        printf("Erro ao alocar localização do membro.\n");
        exit(1);
    }

    newMember->modificationDate = NULL;
    newMember->modificationDate = malloc(sizeof(char) * 20);
    if (! newMember->modificationDate){
        printf("Erro ao alocar data de modificação do membro.\n");
        exit(1);
    }

    newMember->modificationDate = 0;
    newMember->uid = 0;
    newMember->permissions = 0;
    newMember->size = 0;
    newMember->order = 0;

    return newMember;
}

member_t *createNewMember(char *filePath)
{
    member_t *newMember = NULL;
    struct stat fileInfo;

    newMember = allocateEmptyMember();

    if (stat(filePath, &fileInfo) == -1){
        printf("Erro ao obter informações do arquivo.\n");
        exit(1);
    }

    char *fileName = strrchr(filePath, '/');
    if (fileName != NULL)
        fileName++;
    else {
        printf("Não foi possível localizar o nome do arquivo.\n");
        exit(1);
    }
    newMember->name = fileName;

    strcpy(newMember->location, filePath); // PERGUNTAR COMO EXATAMENTE É PRA GUARDAR O CAMINHO
    // se for caminho absoluto, transformar em relativo
    /* if (isAbsolutePath(newMember->location))
        newMember->location = */

    time_t modificationTime = fileInfo.st_mtime;
    newMember->modificationDate = ctime(&modificationTime);

    newMember->uid = fileInfo.st_uid;

    newMember->permissions = fileInfo.st_mode;

    newMember->size = fileInfo.st_size;

    newMember->next = NULL;

    // printMember(newMember);
    return newMember;
}

void freeMember(member_t *member)
{
    free(member->name);
    free(member->location);
    free(member->modificationDate);
    free(member);
}

/* ------------------ EXTERNAL FUNCTIONS ------------------ */

directory_t *createDirectory()
{
    directory_t *newDirectory = NULL;
    newDirectory = malloc(sizeof(directory_t));
    if (! newDirectory){
        printf("Erro ao alocar novo diretório.\n");
        exit(1);
    }

    newDirectory->count = 0;
    newDirectory->members = NULL;

    return newDirectory;
}

directory_t *addMemberToDirectory(directory_t *directory, char *filePath)
{
    member_t *newMember = NULL;

    newMember = createNewMember(filePath);

    if (directory->members == NULL){
        directory->members = newMember;
    } else {
        member_t *aux = directory->members;
        while (aux->next != NULL){
            aux = aux->next;
        }
        aux->next = newMember;
    }
    newMember->order = directory->count;
    directory->count++;

    return directory;
}

void printAllMembersFromDir(directory_t *dir)
{
    member_t *curMember = dir->members;

    while (curMember != NULL)
    {
        printf("-----------------------------\n");
        printf("name: %s\n", curMember->name);
        printf("location: %s\n", curMember->location);
        printf("modificationDate: %s\n", curMember->modificationDate);
        printf("uid: %d\n", curMember->uid);
        printf("permissions: %d\n", curMember->permissions);
        printf("size: %d\n", curMember->size);
        printf("order: %d\n", curMember->order);
        printf("-----------------------------\n");
        printf("\n");
        curMember = curMember->next;
    }
}