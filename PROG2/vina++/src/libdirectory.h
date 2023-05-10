#ifndef _LIB_DIRECTORY_
#define _LIB_DIRECTORY_

typedef struct member {
    char *name; // array size 256
    char *location; // array size 256 *se tiver um path com mais de 256 vsf né
    time_t modificationDate;
    int uid;
    int permissions;
    int size;
    int order;
} member_t;

typedef struct directory {
    int count; // quantidade de membros no arquivo 
    member_t *members; // Lista de membros no arquivo
} directory_t;

#endif