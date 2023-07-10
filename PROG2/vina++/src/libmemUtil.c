#include <stdio.h>
#include <stdlib.h>

#include "libmemUtil.h"
#include "libutil.h"

void updateListPositions(directory *dir);
void updatePositionsBkp(FILE *bkp, directory *dir);
void updateBkpPositions(directory *dir);
int getFilesTotalSize(directory *dir);
member *getMemberByPositionInList(directory *dir, int positionList);
member *getMemberByPath(directory *dir, char *path);
void writeMember(FILE *bkp, member *m);
void printMember(member *member);
void printDirectory(directory *dir);

// Atualiza as posições de lista dos membros de um diretorio (member->positionList)
void updateListPositions(directory *dir)
{
    member *currentMember = dir->head;
    int i = 0;

    while (currentMember != NULL) {
        currentMember->positionList = i;
        currentMember = currentMember->next;
        i++;
    }
}

// Seguem duas funções com mesmo objetivo, mas usadas em situações diferentes
// Atualiza as posicoes no backup dos membros de um diretorio (member->positionBkp)
void updateBkpPositions(directory *dir)
{
    member *m = dir->head;
    int pos = 4;

    while (m != NULL) {
        m->positionBkp = pos;
        pos += m->size;
        m = m->next;
    }
}

// De acordo com o conteudo no bkp atualiza o positionBkp nos membros do dir
void updatePositionsBkp(FILE *bkp, directory *dir)
{
    member *currMember = dir->head;

    fseek(bkp, 0, SEEK_SET);
    fseek(bkp, sizeof(int), SEEK_CUR); // pula o int do inicio

    while (currMember != NULL) {
        currMember->positionBkp = ftell(bkp);
        fseek(bkp, currMember->size, SEEK_CUR);
        currMember = currMember->next;
    }
}

// Retorna o tamanho total dos arquivos de um diretorio
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

// Retorna o membro de um diretorio pelo sua posicao na lista (member->positionList)
member *getMemberByPositionInList(directory *dir, int positionList)
{
    member *currentMember = dir->head;

    while (currentMember != NULL) {
        if (currentMember->positionList == positionList)
            return currentMember;
        currentMember = currentMember->next;
    }

    return NULL;
}

// Retorna o membro de um diretorio pelo seu caminho (member->path)
member *getMemberByPath(directory *dir, char *path)
{
    member *currentMember = dir->head;
    char *relativePath = strdup(path);
    relativePath = getRelativePath(relativePath);

    while (currentMember != NULL) {
        if (strcmp(currentMember->path, relativePath) == 0)
            return currentMember;
        currentMember = currentMember->next;
    }

    return NULL;
}

// Escreve o conteudo de um membro em um arquivo
void writeMember(FILE *bkp, member *m)
{
    fwrite(m->path, 1, PATH_MAX_SIZE, bkp);
    fwrite(m->name, 1, NAME_MAX_SIZE, bkp);
    fwrite(&m->modificationDate, 1, sizeof(time_t), bkp);
    fwrite(&m->size, 1, sizeof(int), bkp);
    fwrite(&m->uid, 1, sizeof(int), bkp);
    fwrite(&m->permissions, 1, sizeof(int), bkp);
    fwrite(&m->positionList, 1, sizeof(int), bkp);
    fwrite(&m->positionBkp, 1, sizeof(unsigned long), bkp);
}

// Imprime um membro no terminal
void printMember(member *member)
{
    printf("Nome: %s\n", member->name);
    printf("Caminho: %s\n", member->path);
    printf("Data de modificação: %ld\n", member->modificationDate);
    printf("Tamanho: %d\n", member->size);
    printf("UID: %d\n", member->uid);
    printf("Permissões: %d\n", member->permissions);
    printf("Posição Lista: %d\n", member->positionList);
    printf("Posição Backup: %ld\n", member->positionBkp);
}

// Imprime no terminal o conteudo do diretorio
void printDirectory(directory *dir)
{
    member *currentMember = dir->head;

    while (currentMember != NULL) {
        printMember(currentMember);
        printf("---\n");
        currentMember = currentMember->next;
    }
}