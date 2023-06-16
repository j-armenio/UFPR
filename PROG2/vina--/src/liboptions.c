#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

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
        insertMemberToDir(dir, argv[i]);
    }

    printf("--------------\n");
    printDirectory(dir);
    printf("--------------\n");
    
    /* Pegar o tamanho total dos arquivos e escrever no inicio do bkp */
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

        char *buffer = (char *) malloc(fileSize);
        if (buffer == NULL){
            printf("Erro ao alocar memória para o buffer.\n");
            exit(1);
        }

        fread(buffer, 1, fileSize, file);
        fwrite(buffer, 1, fileSize, backup);

        free(buffer);
        fclose(file);
    }

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