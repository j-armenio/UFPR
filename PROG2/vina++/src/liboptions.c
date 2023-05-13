#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "liboptions.h"
#include "libdirectory.h"
#include "libbackup.h"

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

        printAllMembersFromDir(directoryStruct);
    }


    fclose(backupFile);
    return 1;
}