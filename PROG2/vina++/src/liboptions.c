#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include "liboptions.h"
#include "libmember.h"
#include "libutil.h"
#include "libbinary.h"
#include "libmemUtil.h"

void insertFilesToBackup(int argc, char **argv);
void insertToNewBackup(int argc, char **argv);
void insertToBackup(int argc, char **argv);
void insertIfNew(int argc, char **argv);
void moveMember(int argc, char **argv);
void extract(int argc, char **argv);
void extractAllFiles(int argc, char **argv);
void extractFiles(int argc, char **argv);
void removeFiles(int argc, char **argv);
void listMembers(int argc, char **argv);
void printHelpMessage();

// -i: inserção de membros no archive
void insertFilesToBackup(int argc, char **argv)
{
    if (argc <= 3){
        printf("Erro ao inserir o comando.\n");
        exit(1);
    }

    if (checkIfFileExists(argv[2]) == 0){
        /* Backup NOVO */
        printf("Backup novo.\n");
        insertToNewBackup(argc, argv);
        return;
    } else {
        /* Backup já existente */
        printf("Backup já existente.\n");
        insertToBackup(argc, argv);
    }
}

void insertToNewBackup(int argc, char **argv)
{
    FILE *backup = fopen(argv[2], "wb+");
    if (backup == NULL){
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }

    // Cria Lista de membros
    directory *dir = createDirectory();

    // Preenche a lista
    int i;
    for (i = 3; i < argc; i++)
    {
        if (checkIfFileExists(argv[i]) == 0){
            printf("O arquivo %s não existe.\n", argv[i]);
            continue;
        }
        member *m = createMember(argv[i]);
        insertMemberToDir(dir, m);
    }
    
    /* Pega o tamanho total dos arquivos e escrever no inicio do bkp */
    int totalSize = getFilesTotalSize(dir);
    fwrite(&totalSize, sizeof(int), 1, backup);

    /* Escrever o conteudo dos arquivos no bkp */
    for (i = 3; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "rb+");
        if (file == NULL){
            printf("Erro ao abrir o arquivo %s.\n", argv[i]);
            exit(1);
        }
        fseek(file, 0, SEEK_END);
        int fileSize = ftell(file);
        rewind(file);

        copyBinary(file, fileSize, backup);

        fclose(file);
    }

    // Atualiza o valor positionBkp dos membros da lista, 
    updatePositionsBkp(backup, dir);

    /* Escreve a quantidade de memberos */
    fwrite(&dir->memberCount, sizeof(int), 1, backup);

    /* Escrever o diretorio no bkp */
    for (i = 0; i < dir->memberCount; i++)
    {
        member *m = getMemberByPositionInList(dir, i);
        writeMember(backup, m);
    }

    fclose(backup);
    freeDirectory(dir);
}

void insertToBackup(int argc, char **argv)
{
    FILE *bkp = fopen(argv[2], "rb+");
    if (bkp == NULL){
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }
    
    directory *dir = readBackupToDirectory(bkp);
    int lastContentByte = dir->tail->positionBkp + dir->tail->size;

    int i;
    for (i = 3; i < argc; i++)
    {
        member *target = getMemberByPath(dir, argv[i]);

        FILE *newFile = fopen(argv[i], "rb+");
        if (newFile == NULL){
            printf("Erro ao abrir o arquivo %s.\n", argv[i]);
            exit(1);
        }
        member *newFileMember = createMember(argv[i]);

        if (target != NULL) // Arquivo existe no backup
        {
            if (newFileMember->size == target->size) { // O novo arquivo tem o mesmo tamanho do antigo
                printf("O arquivo %s tem o mesmo tamanho que o arquivo antigo.\n", argv[i]);
            } 
            else { // O novo arquivo tem um tamanho diferente do antigo
                int blockSize = lastContentByte - (target->positionBkp + target->size);

                // Move o bloco para o final do arquivo
                pasteMember(
                    bkp,
                    target->positionBkp + target->size, 
                    blockSize,
                    lastContentByte
                );

                pasteMember(bkp, lastContentByte, blockSize, target->positionBkp + newFileMember->size);

                fseek(bkp, target->positionBkp, SEEK_SET);
                copyBinary(newFile, newFileMember->size, bkp);

                if (newFileMember->size > target->size){
                    printf("O arquivo %s no backup será substituido.\n", argv[i]);

                    ftruncate(fileno(bkp), lastContentByte + (newFileMember->size - target->size));

                    // Escreve a quantidade de membros
                    fseek(bkp, 0, SEEK_END);
                    fwrite(&dir->memberCount, sizeof(int), 1, bkp);

                    // Atualiza a posicao dos membros
                    member *m = target->next;
                    int diff = newFileMember->size - target->size;
                    while (m != NULL){
                        m->positionBkp += diff;
                        m = m->next;
                    }
                } else if (newFileMember->size < target->size){
                    ftruncate(fileno(bkp), lastContentByte - (target->size - newFileMember->size));

                    // Escreve a quantidade de membros
                    fseek(bkp, 0, SEEK_END);
                    fwrite(&dir->memberCount, sizeof(int), 1, bkp);

                    // Atualiza a posicao dos membros
                    member *m = target->next;
                    int diff = target->size - newFileMember->size;
                    while (m != NULL){
                        m->positionBkp -= diff;
                        m = m->next;
                    }
                }

                target->size = newFileMember->size;
                target->modificationDate = newFileMember->modificationDate;
                target->uid = newFileMember->uid;
                target->permissions = newFileMember->permissions;

                fseek(bkp, 0, SEEK_END);
                
                // Escreve o diretorio no bkp
                member *m = dir->head;
                while (m != NULL) {
                    writeMember(bkp, m);
                    m = m->next;
                }

                // Atualizar o int no inicio do bkp
                fseek(bkp, 0, SEEK_SET);
                int totalSize = getFilesTotalSize(dir);
                fwrite(&totalSize, sizeof(int), 1, bkp);
            }
        }
        else // Arquivo nao existe no backup
        {
            fseek(bkp, lastContentByte, SEEK_SET);
            copyBinary(newFile, newFileMember->size, bkp);
            
            newFileMember->positionBkp = lastContentByte;
            insertMemberToDir(dir, newFileMember);
            
            // Escreve a quantidade de membros
            fseek(bkp, dir->tail->positionBkp + dir->tail->size, SEEK_SET);
            fwrite(&dir->memberCount, sizeof(int), 1, bkp);

            // Escreve o diretorio no bkp
            member *m = dir->head;
            while (m != NULL) {
                writeMember(bkp, m);
                m = m->next;
            }

            // Atualizar o int no inicio do bkp
            fseek(bkp, 0, SEEK_SET);
            int totalSize = getFilesTotalSize(dir);
            fwrite(&totalSize, sizeof(int), 1, bkp);
        }
    }

    fclose(bkp);
    freeDirectory(dir);
}

// -a: inserção SE o membro for mais recente que o arquivado
void insertIfNew(int argc, char **argv)
{
    FILE *bkp = fopen(argv[2], "rb+");
    if (bkp == NULL){
        // O arquivo de backup nao existe
        insertToNewBackup(argc, argv);
    } else {
        directory *dir = readBackupToDirectory(bkp);

        int newArgc = 3;
        char **newArgv = malloc(sizeof(char *) * argc);
        newArgv[0] = argv[0];
        newArgv[1] = argv[1];
        newArgv[2] = argv[2];

        int i;
        for (i = 3; i < argc; i++) 
        {
            member *target = getMemberByPath(dir, argv[i]);
            member *newMember = createMember(argv[i]);

            if (target != NULL) { // Arquivo existe no backup
                if (newMember->modificationDate > target->modificationDate) { // newMember eh mais recente
                    printf("O arquivo %s no backup será substituido.\n", argv[i]);
                    newArgv[newArgc] = argv[i];
                    newArgc++;
                } else { // target eh mais recente
                    // Nao faz nada
                }
            } else { // Arquivo nao existe
                newArgv[newArgc] = argv[i];
                newArgc++;
            }
        }
        insertToBackup(newArgc, newArgv);
        freeDirectory(dir);
    }

    fclose(bkp);
}

// -m: move o membro indicado para imediatamente depois do membro target
void moveMember(int argc, char **argv)
{
    FILE *bkp = fopen(argv[3], "rb+");
    if (bkp == NULL) {
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }
    directory *dir = readBackupToDirectory(bkp);

    member *target = getMemberByPath(dir, argv[2]);
    member *memberToMove = getMemberByPath(dir, argv[4]);

    if (target == NULL || memberToMove == NULL) {
        printf("Erro ao mover o arquivo.\n");
        printf("Arquivo não existe ou seu caminho está incorreto.\n");
        exit(1);
    }

    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    int lastContentByte = dir->tail->positionBkp + dir->tail->size;

    if (target->positionBkp < memberToMove->positionBkp) {
        // Copia o memberToMove para depois do ultimo byte de binario
        pasteMember(bkp, memberToMove->positionBkp, memberToMove->size, lastContentByte);

        // Shift members entre target e memberToMove para a direita
        shiftRight(
            bkp,
            target->positionBkp + target->size,
            memberToMove->positionBkp,
            memberToMove->positionBkp + memberToMove->size
        );

        // Copiar o memberToMove para logo depois do target
        pasteMember(bkp, lastContentByte, memberToMove->size, target->positionBkp + target->size);
    } else if (target->positionBkp > memberToMove->positionBkp) { 
        // Copia o memberToMove para depois do ultimo byte de binario
        pasteMember(bkp, memberToMove->positionBkp, memberToMove->size, lastContentByte);

        // Shift members entre target e memberToMove para a direita
        shiftLeft(
            bkp,
            memberToMove->positionBkp + memberToMove->size,
            target->positionBkp + target->size,
            memberToMove->positionBkp
        );
        int curPos = ftell(bkp);
        
        // Copiar o memberToMove para logo depois do target
        pasteMember(bkp, lastContentByte, memberToMove->size, curPos);
    } else {
        printf("Erro ao mover o arquivo.\n");
        printf("O arquivo de destino e o arquivo a ser movido são o mesmo.\n");
        exit(1);
    }

    // Trunca o arquivo para o tamanho do ultimo byte de binario
    ftruncate(fileno(bkp), lastContentByte);
    // Faz o update da lista
    shiftMoveInList(dir, target, memberToMove);

    // Escreve a quantidade de membros
    fseek(bkp, 0, SEEK_END);
    fwrite(&dir->memberCount, sizeof(int), 1, bkp);

    // Escreve o diretorio no bkp
    member *m = dir->head;
    while (m != NULL) {
        writeMember(bkp, m);
        m = m->next;
    }
        
    free(buffer);
    fclose(bkp);
}

// -x: extrai os membros indicados de archive
void extract(int argc, char **argv)
{
    if (argc == 3){
        printf("Extrai todos os membros\n");
        extractAllFiles(argc, argv);
    } else {
        extractFiles(argc, argv);
    }
}

void extractAllFiles(int argc, char **argv)
{
    FILE *bkp = fopen(argv[2], "rb+");
    if (bkp == NULL) {
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }
    directory *dirList = readBackupToDirectory(bkp);

    // Cria um diretorio com o mesmo nome do backup sem a extensao
    char *dirName = getRelativePath(argv[2]);
    removeExtension(dirName);

    int result = mkdir(dirName, 0777);
    if (result == -1) {
        printf("Erro ao criar o diretório.\n");
        exit(1);
    }

    result = chdir(dirName);
    if (result == -1) {
        printf("Erro ao entrar no diretório.\n");
        exit(1);
    }

    extractContent(bkp, dirList);

    fclose(bkp);
    freeDirectory(dirList);
}

void extractFiles(int argc, char **argv)
{
    FILE *bkp = fopen(argv[2], "rb+");
    if (bkp == NULL) {
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }
    directory *dirList = readBackupToDirectory(bkp);

    // Cria um diretorio com o mesmo nome do backup sem a extensao
    /* char *dirName = getRelativePath(argv[2]);
    removeExtension(dirName);

    int result = mkdir(dirName, 0777);
    if (result == -1) {
        printf("Erro ao criar o diretório.\n");
        exit(1);
    }
    result = chdir(dirName);
    if (result == -1) {
        printf("Erro ao entrar no diretório.\n");
        exit(1);
    } */

    int i;
    for (i = 3; i < argc; i++) 
    {
        member *target = getMemberByPath(dirList, argv[i]);
        if (target == NULL) {
            printf("Erro ao extrair o arquivo.\n");
            printf("Arquivo não existe ou seu caminho está incorreto.\n");
            exit(1);
        }

        createDirectories(target->path);
        FILE *newFile = fopen(target->path, "wb");
        if (newFile == NULL) {
            printf("Erro ao criar o arquivo.\n");
            exit(1);
        }

        fseek(bkp, target->positionBkp, SEEK_SET);
        copyBinary(bkp, target->size, newFile);
        fclose(newFile);
    }
    fclose(bkp);
    freeDirectory(dirList);
}

// -r: remove os membros indicados de archive
void removeFiles(int argc, char **argv)
{
    if (argc < 4) {
        printf("Erro ao remover o arquivo.\n");
        printf("Argumentos insuficientes.\n");
        exit(1);
    }

    FILE *bkp = fopen(argv[2], "rb+");
    if (bkp == NULL) {
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }
    directory *dir = readBackupToDirectory(bkp);

    if (dir->head->next == NULL) { // Apenas um arquivo no backup
        if (strcmp(dir->head->name, argv[3]) == 0) { // Arquivo existe
            ftruncate(fileno(bkp), 0);
            fclose(bkp);
            exit(0);
        } else { // Arquivo nao existe
            printf("Erro ao remover o arquivo.\n");
            printf("Arquivo não existe ou seu caminho está incorreto.\n");
            exit(1);
        }
    } else {
        int i;
        for (i = 3; i < argc; i++) 
        {
            member *target = getMemberByPath(dir, argv[i]);
            if (target == NULL) {
                printf("Erro ao remover o arquivo.\n");
                printf("Arquivo não existe ou seu caminho está incorreto.\n");
                exit(1);
            }
            int lastContentByte = dir->tail->positionBkp + dir->tail->size;

            pasteMember(
                bkp, 
                target->positionBkp + target->size, 
                lastContentByte - (target->positionBkp + target->size), 
                target->positionBkp
            );

            ftruncate(fileno(bkp), lastContentByte - target->size);

            removeMemberFromDir(dir, target);

            // Escreve a quantidade de membros
            fseek(bkp, 0, SEEK_END);
            fwrite(&dir->memberCount, sizeof(int), 1, bkp);

            // Escreve o diretorio no bkp
            member *m = dir->head;
            while (m != NULL) {
                writeMember(bkp, m);
                m = m->next;
            }

            // Atualiza o int no começo do backup
            fseek(bkp, 0, SEEK_SET);
            int totalSize = getFilesTotalSize(dir);
            fwrite(&totalSize, sizeof(int), 1, bkp);
        }

        fclose(bkp);
    }
    freeDirectory(dir);
}

// -c: lista os membros de archive
void listMembers(int argc, char **argv)
{
    FILE *bkp = fopen(argv[2], "rb+");
    if (bkp == NULL) {
        printf("Erro ao abrir o arquivo de backup.\n");
        exit(1);
    }

    directory *dir = readBackupToDirectory(bkp);
    
    struct stat fileInfo;
    if (stat(argv[2], &fileInfo) != 0) {
        printf("Erro ao obter informações do arquivo.\n");
        exit(1);
    }

    struct passwd *pw = getpwuid(fileInfo.st_uid);
    if (pw == NULL) {
        printf("Erro ao obter informações do usuário.\n");
        exit(1);
    }
    const char* userName = pw->pw_name;

    struct group *gr = getgrgid(fileInfo.st_gid);
    if (gr == NULL) {
        printf("Erro ao obter informações do grupo.\n");
        exit(1);
    }
    const char *groupName = gr->gr_name;

    time_t bkpModificationDate = fileInfo.st_mtime;
    struct tm *timeInfo = localtime(&bkpModificationDate);
    char bkpModificationDateStr[20];
    strftime(bkpModificationDateStr, 20, "%Y-%m-%d %H:%M", timeInfo);

    // permissoes é %-10
    printf("drwxrwxr-x %s/%-s         0 %s %s/\n", userName, groupName, bkpModificationDateStr, argv[2]);

    member *m = dir->head;
    int i;
    for (i = 0; i < dir->memberCount; i++)
    {
        char *octalMode = getOctalMode(m->permissions);
        
        struct tm *timeInfo = localtime(&m->modificationDate);
        char modificationDateStr[20];
        strftime(modificationDateStr, 20, "%Y-%m-%d %H:%M", timeInfo);

        printf("-%s %s/%-s %9d %s %s\n", octalMode, userName, groupName, m->size, modificationDateStr, m->path);

        m = m->next;
    }

    fclose(bkp);
    freeDirectory(dir);
}

// -h: imprime a mensagem de ajuda
void printHelpMessage()
{
    printf(
        "\n"
        "As seguintes opções estão disponíveis para o programa vina++,\n"
        "seguindo o formato:\n"
        "\n"
        "  ./vina++ <opção> <archive> [membro1 membro2 ...]\n"
        "\n"
        "=======================================================================\n"
        "\n"
        "  -i : insere/acresenta um ou mais membros ao archive. Caso o membro\n"
        "  já exista no archive, ele deve ser substituído.\n"
        "\n"
        "  -a : mesmo comportamento da opção -i, mas a substituição de um\n"
        "  membro existente ocorre APENAS caso o parâmetro seja mais recente\n"
        "  que o arquivado.\n"
        "\n"
        "  -m target: move o membro indicado na linha de comando para\n"
        "  imediatamente depois do membro target existente em archive.\n"
        "\n"
        "  -x: extrai os membros indicados de archive. Se os membros não forem\n"
        "  indicados, todos devem ser extraídos.\n"
        "\n"
        "  -r: remove os membros indicados de archive.\n"
        "\n"
        "  -c: lista o conteúdo de archive em ordem, incluindo as propriedades\n"
        "  de cada membro (nome, UID, permissões, tamanho e data de\n"
        "  modificação) e sua ordem no arquivo.\n"
        "\n"
        "  -h: imprime uma pequena mensagem de ajuda com as opções disponíveis\n"
        "  e encerra.\n"
        "\n"
        "=======================================================================\n"
        "\n"
        );
}
