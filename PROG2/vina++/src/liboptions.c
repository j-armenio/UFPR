#include <stdio.h>
#include <stdlib.h>

#include "liboptions.h"

int printHelpMessage()
{
    printf("As seguintes opções estão disponíveis para o programa vina++,\n"
        "seguindo o formato:\n"
        "  vina++ <opção> <archive> [membro1 membro2 ...]\n"
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
        "  de cada membro (nome, UID, permissões, tamanho e data de modificação)\n"
        "  e sua ordem no arquivo.\n"
        "\n"
        "  -h: imprime uma pequena mensagem de ajuda com as opções disponíveis"
        "  e encerra.\n"
        "\n"
        "=======================================================================\n");
    
    return 0;
}