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
    // 1. Criar o diretorio
    directory_t *directoryStruct = NULL;
    directoryStruct = createDirectory();

    // 2. Crir o arquivo de backup
    FILE *backupFile = fopen(argv[2], "wb");
    if (! backupFile){
        printf("Erro ao criar arquivo de backup.\n");
        exit(1);
    }

    // 3. Fazer um loop, que a cada iteração passa por um dos arquivos a ser comprimido
    int i;
    for (i = 3; i < argc; i++)
    {
        // 4. Le o content do file e insere esse content no backup
        contentFromFileToBackup(backupFile, argv[i]);

        // 5. Atualiza as informações da structDir
        addMemberToDirectory(directoryStruct, argv[i]);
    }

    // printAllMembersFromDir(directoryStruct);

    fseek(backupFile, 0, SEEK_END);

    // 6. Passa o conteudo do dir para o backup
    member_t *curMember = directoryStruct->members;

    // 7. Escreve o header no inicio da area de diretorio do backup: <HEADER BACKUP>
    fwrite("<HEADER BACKUP>", sizeof(char), 15, backupFile);

    // 8. Escreve o conteudo do dir no backup
    while (curMember != NULL)
    {
        // 8.1 Escreve o header do membro no inicio da area de diretorio do backup: <HEADER MEMBER>
        fwrite("<HEADER MEMBER>", sizeof(char), 15, backupFile);

        // 8.2 Escreve os dados do membro no backup
        fwrite(curMember->name, sizeof(char), strlen(curMember->name) , backupFile);
        // printf("Nome: %s\n", curMember->name);
        fwrite(curMember->location, sizeof(char), strlen(curMember->location), backupFile);
        // printf("Localização: %s\n", curMember->location);
        fwrite(curMember->modificationDate, sizeof(char), 20, backupFile);
        // printf("Data de modificação: %s", curMember->modificationDate);
        fwrite(&(curMember->uid), sizeof(int), 1, backupFile);
        // printf("uid: %d\n", curMember->uid);
        fwrite(&(curMember->permissions), sizeof(int), 1, backupFile);
        // printf("permissões: %d\n", curMember->permissions);
        fwrite(&(curMember->size), sizeof(int), 1, backupFile);
        // printf("tamanho: %d\n", curMember->size);
        fwrite(&(curMember->order), sizeof(int), 1, backupFile);
        // printf("ordem: %d\n", curMember->order);
        
        curMember = curMember->next;
    }

    fclose(backupFile);
    return 1;
}

void extractFiles(int argc, char **argv)
{

}

void extractAllFiles(char *directoryPath)
{

}