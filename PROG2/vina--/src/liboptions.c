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

/* --- Prototipos --- */

void insertToNewBackup(int argc, char **argv);
void insertToBackup(int argc, char **argv);

/* --- Internas --- */

/* --- Externas --- */

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

}

void insertFilesToBackup(int argc, char **argv)
{
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

    directory *dir = createDirectory();
    if (dir == NULL){
        printf("Erro ao criar o diretório.\n");
        exit(1);
    }

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

    printf("--------------\n");
    printDirectory(dir);
    printf("--------------\n");
    
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

        /* !!! BUFFER MÁXIMO DE 1024 !!! */
        char *buffer = (char *) malloc(sizeof(char) * 1024);
        if (buffer == NULL){
            printf("Erro ao alocar memória para o buffer.\n");
            exit(1);
        }

        int iterations = fileSize / 1024;
        int bytesLeft = fileSize % 1024;

        int j;
        for (j = 0; j < iterations; j++) 
        {
            fread(buffer, 1, 1024, file);
            fwrite(buffer, 1, 1024, backup);
        }
        fread(buffer, 1, bytesLeft, file);
        fwrite(buffer, 1, bytesLeft, backup);

        free(buffer);
        fclose(file);
    }

    /* Escreve a quantidade de memberos */
    fwrite(&dir->memberCount, sizeof(int), 1, backup);

    /* Escrever o diretorio no bkp */
    for (i = 0; i < dir->memberCount; i++)
    {
        member *m = getMemberByPosition(dir, i);
        writeMember(backup, m);
    }
}

void insertToBackup(int argc, char **argv)
{
    
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

    extractToDir(bkp, dirList); 

}

void extractFiles(int argc, char **argv)
{

}