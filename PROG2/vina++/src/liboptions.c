#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "liboptions.h"
#include "libdirectory.h"
#include "libbackup.h"
#include "libutil.h"

int printHelpMessage()
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
    
    return 0;
}

int insertFilesIntoBackup(int argc, char **argv)
{
    directory_t *directoryStruct = NULL;
    directoryStruct = createDirectory();

    FILE *backupFile = fopen(argv[2], "wb");
    if (! backupFile) {
        printf("Erro ao criar arquivo de backup.\n");
        exit(1);
    }

    // Pega as infos dos membros para o dir
    int i;
    for (i = 3; i < argc; i++)
        addMemberToDirectory(directoryStruct, argv[i]);
    
    unsigned int totalSize = 0;
    member_t *aux = directoryStruct->members;

    // Insere um int com o tamanho antes do dir
    while (aux != NULL) {
        totalSize += aux->size;
        aux = aux->next;
    }
    fwrite(&totalSize, sizeof(int), 1, backupFile);

    // Le o content do file e insere esse content no backup
    for (i = 3; i < argc; i++)
        FileContentToBackup(backupFile, argv[i]);

    char *lineBreak = "\n";

    // Imprime o dir para o backup
    member_t *curMember = directoryStruct->members;
    while (curMember != NULL)
    {
        fwrite("<HEADER MEMBER>", sizeof(char), 15, backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(curMember->name, sizeof(char), strlen(curMember->name) , backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(curMember->location, sizeof(char), strlen(curMember->location), backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(curMember->modificationDate, sizeof(char), 20, backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(&(curMember->uid), sizeof(int), 1, backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(&(curMember->permissions), sizeof(int), 1, backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(&(curMember->size), sizeof(int), 1, backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        fwrite(&(curMember->order), sizeof(int), 1, backupFile);
        fwrite(lineBreak, sizeof(char), 1, backupFile);
        
        curMember = curMember->next;
    }

    printAllMembersFromDir(directoryStruct);

    fclose(backupFile);
    return 1;
}

void extractAllFiles(char *backupPath)
{
    directory_t *directoryStruct = NULL;
    directoryStruct = createDirectory();

    FILE *backupFile = fopen(backupPath, "rb");
    if (! backupFile) {
        printf("Erro ao abrir arquivo de backup.\n");
        exit(1);
    }

    // Pega o tamanho total do backup
    unsigned int binarySize = 0;
    fread(&binarySize, sizeof(int), 1, backupFile);
    printf("Binary size: %d\n", binarySize);

    // Vai ate a parte do diretorio do arquivo
    fseek(backupFile, binarySize, SEEK_SET);

    char *buffer = NULL;

    int c;
    while ((c = fgetc(backupFile)) != EOF)
        printf("%c", c);

    // fread(buffer, sizeof(char), 15, backupFile);
    // printf("Buffer: %s\n", buffer);

    // Le o dir do backup e insere no directoryStruct
    // char *header = "<HEADER MEMBER>";
    // char *buffer = NULL;

    // fread(buffer, 1, 15, backupFile);

    /* while (fread(buffer, sizeof(char), 15, backupFile) != 0)
    {
        if (strcmp(buffer, header) == 0) {
            member_t *newMember = allocateEmptyMember();
            fread(newMember->name, sizeof(char), 100, backupFile);
            fseek(backupFile, 1, SEEK_CUR);
            fread(newMember->location, sizeof(char), 100, backupFile);
            fseek(backupFile, 1, SEEK_CUR);
            fread(newMember->modificationDate, sizeof(char), 20, backupFile);
            fseek(backupFile, 1, SEEK_CUR);
            fread(&(newMember->uid), sizeof(int), 1, backupFile);
            fseek(backupFile, 1, SEEK_CUR);
            fread(&(newMember->permissions), sizeof(int), 1, backupFile);
            fseek(backupFile, 1, SEEK_CUR);
            fread(&(newMember->size), sizeof(int), 1, backupFile);
            fseek(backupFile, 1, SEEK_CUR);
            fread(&(newMember->order), sizeof(int), 1, backupFile);
            fseek(backupFile, 1, SEEK_CUR);

            addMemberToDirectory(directoryStruct, newMember->name);
        }
    } */

}

void extractFiles(int argc, char **argv)
{

}