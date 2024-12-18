### Meu protocolo

| type | data | from | acks |

---

# Passos do jogo

1. **Distribuição inicial das cartas**: Cada jogador (incluindo o delaer) recebe duas cartas iniciais. As cartas dos jogadores são geralmente visíveis para todos, enquanto o dealer tem uma carta virada para cima e uma carta virada para baixo.

2. **Checagem de Blackjack**: Checa para ver se algum jogador tiver uma pontuação de 21 com as duas cartas iniciais (um Blackjack "natural").

3. **Turno de cada jogador**: Cada jogado toma decisões até *Stand* ou "Estourar"(ultrapassar 21 pontos). O jogador com blackjack natural não joga.

4. **Turno do dealer**: Após os jogadores finalizarem suas jogadas, o dealer revela a carta oculta. O dealer deve seguir regras fixas:
    - Hit alcanca no mínimo 17 pontos.
    - Stand ao alcançar 17 pontos ou mais
Se o dealer ultrapassar 21 pontos, ele estoura, e todos jogadores que ainda estão no jogo ganham a rodada.

Caso alguem tenha, o seguinte acontece: O Dealer checa **secretamente** a carta dele. 
    * Se for um **blackjack**, ele revela a todos, *empata*(push) com os jogadores que tiveram um blackjack natural e ganha dos demais.
    * Se não for um **blackjack**, o jogador com o bj natural ganha o round, a segunda carta continua escondida, e os demais jogam normalmente.

5. **Comparação de mão e pagamento**: As mãos dos jogadores são comparadas à mão do dealer.
    - Vitória do Jogador: Se o jogador tiver uma pontuação mais alta que o dealer sem ultrapassar 21, ele vence e recebe o pagamento. Nesse caso o jogador ganha um valor igual a aposta. (aposta $10 recebe $20)
    - Derrota do Jogador: Se o dealer tiver uma pontuação maior, o jogador perde sua aposta. Perde tudo.
    - Empate (push): Se a pontuação do jogador for igual a do dealer, o jogador recupera sua aposta inicial.
    - Blackjack: Se um jogador tiver um 21, ele ganha 1,5 vezes sua aposta, exceto se o dealer também tiver um Blackjack, caso em que é um empate. 

# Passos do jogo na Rede

1. Dealer manda uma mensagem perguntando quanto cada um quer apostar. Jogadores preenchem as mensagens e volta pro dealer. Créditos de cada jogador é atualizado individualmente quando mandam quanto querem apostar. 

1. Dealer distribui duas cartas para cada jogador e duas para si mesmo. Ele manda uma mensagem para toda rede avisando quais são as cartas de cada um. (lembrando de mostrar apenas uma sua, ocultando a outra).

2. Caso alguém tem blackjack
    1. Dealer desvira a carta dele. Se for um blackjack, é um empate. Se não for, a pessoa com blackjack ganha. Dealer envia mensagem avisando todos o resultado. 
    2. Após a mensagem voltar pra ele, o dealer envia uma mensagem dizendo os pagamentos de cada jogador.

2. Caso normal = loop de mensagens até todos jogadores precisarem dar *Stand*.
Dealer manda mensagem perguntando quais são as ações de cada jogador.

3. 

# Destaques

* O único campo da mensagem que os jogadores podem alterar é a *data* e *acks*, de resto apenas o Dealer altera.
* Apenas o Dealer coloca e retira mensagens da rede.

# Tipos de mensagem

Protocolo:
    message = {
        "type": "players-bet",
        "from": player_id,
        "data": [],
        "acks": [0, 0, 0, 0]
    }

- players-bet
    * DATA: Uma lista onde cada elemento é uma aposta de um jogador. Jogador i aposta no `data[i]`.

- distribute-cards
    * DATA: Um dicionario com as cartas de cada jogador, usando o player_id como chave

- get-actions
    * DATA: um vetor, onde cada indice i contém uma tupla (ação, card=None), card só é atualizado pelo Dealer, e quando vem a ação HIT
    Códigos de ação:
        STAND,
        HIT,
        SURRENDER,
        NATURAL : avisa que o jogador ganhou com um bj natural,
        BUST: avisa que o jogador estorou os pontos (>21)

- result-payment
    * DATA: um vetor, onde cada indice contém uma lista [resultado, pagamento]. O indice do dealer, possui a mão do Dealer no final da rodada.
    Códigos de resultado:
        SURRENDER : retorna metade do valor da aposta,
        WIN : retorna 200% do valor da bet,
        LOSE : retorna 0,
        TIE : retorna bet

- inform-dealer
    * DATA: um int que diz o id do dealer atual

- baton: Apenas uma marcador para passar o bastão da rede em anel. !! Essa é a única mensagem que não precisa ser retirada por quem enviou !!
    * DATA: None

5. **Comparação de mão e pagamento**: As mãos dos jogadores são comparadas à mão do dealer.
    - Vitória do Jogador: Se o jogador tiver uma pontuação mais alta que o dealer sem ultrapassar 21, ele vence e recebe o pagamento. Nesse caso o jogador ganha um valor igual a aposta. (aposta $10 recebe $20)
    - Derrota do Jogador: Se o dealer tiver uma pontuação maior, o jogador perde sua aposta. Perde tudo.
    - Empate (push): Se a pontuação do jogador for igual a do dealer, o jogador recupera sua aposta inicial.
    - Blackjack: Se um jogador tiver um 21, ele ganha 1,5 vezes sua aposta, exceto se o dealer também tiver um Blackjack, caso em que é um empate.


# Ações do Player

- chosing action
    - *Stand*(parar): Se o jogador está satisfeito com sua mão e não quer pedir mais cartas.
    - *Hit*(acertar): Se o jogador deseja mais uma carta
    - *Double*(dobrar)(OPCIONAL): Se o jogador sentir que necessita de uma e somente uma carta adicional, então pode dobrar sua aposta e receber mais uma carta, boa ou ruim. Esta opção é oferecida somente nas duas primeiras cartas, e às vezes nas duas primeiras cartas após *split*.
    - *Split*(dividir)(OPCIONAL): Se as primeiras duas cartas do jogador forem de mesmo valor em pontos, ele pode dividi-las em duas mãos. Neste envento cada carta é a primeira carta de uma mão nova. O jogador deve também fazer uma outra aposta de valor igual à primeira para a seguinda mão. O jogador pode geralmente dividir até duas ou três vezes consecutivas se aparecer a oportunidade. Dobrar após dividir pode ser permitido, mas nem sempre é o caso.
    - *Surrender*(rendição)(OPCIONAL): Alguns cassinos oferecem ao jogador a opção de rendição nas primeiras duas cartas. Se o jogador não gostar das duas cartas pode perder metade do valor da aposta assim como suas cartas. Esta opção geralmente é oferecida somente depois que o negociante verifica se há o blackjack.
