#ifndef _LIB_MEMBER_
#define _LIB_MEMBER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PATH_MAX_SIZE 1024
#define NAME_MAX_SIZE 256

typedef struct member {
    char path[PATH_MAX_SIZE];
    char name[NAME_MAX_SIZE];
    time_t modificationDate;
    int size;
    int uid;
    int permissions;
    int positionList; // posicao do membro na lista
    unsigned long positionBkp; // posicao do conteudo do membro (em bytes) no bkp
    struct member *next;
} member;

typedef struct {
    member *head; // começo da lista de membros
    member *tail; // fim da lista
    unsigned int memberCount; // quantidade de membros
} directory;

directory *createDirectory();
void freeDirectory(directory *dir);
void printDirectory(directory *dir);
member *createMember(char *path);
directory *insertMemberToDir(directory *dir, member *newMember);
int getFilesTotalSize(directory *dir);
member *getMemberByPositionInList(directory *dir, int positionList);
void writeMember(FILE *bkp, member *m);
directory *readBackupToDirectory(FILE *bkp);
void extractContent(FILE *bkp, directory *dirList);
void updatePositionsBkp(FILE *bkp, directory *dir);
member *getMemberByPath(directory *dir, char *path);
void shiftMoveInList(directory *lst, member *m1, member *m2);
void printMember(member *member);

#endif