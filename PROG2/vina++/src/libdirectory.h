#ifndef _LIB_DIRECTORY_
#define _LIB_DIRECTORY_

// Cada member tem toda informação de um arquivo que está no backup
typedef struct member {
    char *name; // array size 256
    char *location; // array size 256 *se tiver um path com mais de 256 vsf né
    char *modificationDate; // array size 20
    int uid;
    int permissions;
    int size;
    int order;
    struct member *next;
} member_t;

// O directoru tem a quantidade de members e aponta para uma lista de members
typedef struct directory {
    int count; // Quantidade de membros no arquivo
    member_t *members; // Lista de membros no arquivo
} directory_t;

void printMember(member_t *member);
member_t *allocateEmptyMember();
member_t *createNewMember(char *filePath);
void freeMember(member_t *member);
directory_t *createDirectory();
directory_t *addMemberToDirectory(directory_t *directory, char *filePath);
void printAllMembersFromDir(directory_t *dir);

#endif