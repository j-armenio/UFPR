#ifndef _LIB_MEMBER_
#define _LIB_MEMBER_

#define NAME_MAX_SIZE 256
#define PATH_MAX_SIZE 1024

typedef struct member {
    char name[NAME_MAX_SIZE];
    char path[PATH_MAX_SIZE];
    char modificationDate[25];
    unsigned int size;
    unsigned int uid;
    unsigned int permissions;
    unsigned int position; // ordem do membro no arquivo (e na lista)
    struct member *next;
    struct member *previous;
} member;

typedef struct {
    member *head; // começo da lista de membros
    member *tail; // fim da lista
    unsigned int memberCount; // quantidade de membros
} directory;

directory *createDirectory();
void freeDirectory(directory *dir);
void printDirectory(directory *dir);

#endif