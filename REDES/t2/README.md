# Redes 1 - T2
---

# Blackjack

* Criar uma rede em anel com 4 máquinas usando Socket DGRAM UDP sobre a rede Ethernet do Dinf
* O controle de acesso a rede deve ser feito por passagem de bastão
* Todas as mensagens devem dar a volta toda pelo anel
* O bastão não é temporizado
* As mensagens não são temporizadas
* Não é necessário timeout
* Pode ser feito em qualquer linguagem de programação
* Cada equipe DEVE criar o seu próprio protocolo e documentar ele no relatório
* Deve ser entregue um relatório impresso com as decisões tomadas pela equipe 
* Em duplas
* Valor: 20
* Enviar um único arquivo .tgz com o código fonte - o nome do arquivo deve ser o GRR dos membros da equipe

---
## Regras do jogo
https://pt.wikipedia.org/wiki/Blackjack

* Cada jogador deve iniciar com uma quantidade inicial de créditos.
* A cada rodada o carteador muda, passa para frente.
* Durante uma rodada, recebe duas cartas inicias, pode pedir mais cartas, um de cada vez. Se estourar perde.

### Links úteis

[Token ring network and how it works](https://www.youtube.com/watch?v=p72R2uGglnU)
- Mostra estrutura do protocolo com bastão

Token frame (controle de acesso)

|Start delimiter|Access Control|End delimiter|

Data/command frame

|Start delimiter|Access control|Frame control|Destination address|Source address|Data|FCS|End delimiter|Frame status|

---
### Meu protocolo (temporário)

| Origem | Tipo | Dados |

!! LEMBRAR DO PIGBACK (ACKS) NO FINAL !!

---

# Passos do jogo

1. **Distribuição inicial das cartas**: Cada jogador (incluindo o delaer) recebe duas cartas iniciais. As cartas dos jogadores são geralmente visíveis para todos, enquanto o dealer tem uma carta virada para cima e uma carta virada para baixo.

2. **Checagem de Blackjack**: Se algum jogador ou o dealer tiver uma pontuação de 21 com as duas cartas iniciais (um Blackjack "natural"), essa pessoa ganha a rodada imediatamente, exceto se o dealer também tiver um Blackjack, caso em que é um empate ("push").

3. **Turno de cada jogador**: Cada jogado toma decisões até *Stand* ou "Estourar"(ultrapassar 21 pontos).

4. **Turno do dealer**: Após os jogadores finalizarem suas jogadas, o dealer revela a carta oculta. O dealer deve seguir regras fixas:
    - Hit alcanca no mínimo 17 pontos.
    - Stand ao alcançar 17 pontos ou mais
Se o delaer ultrapassar 21 pontos, ele estoura, e todos jogadores que ainda estão no jogo ganham a rodada.

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

- inform-turn
    * DATA: informa quais são as cartas do Dealer. Usado quando o Dealer vira a carta dele.

- get-actions
- inform-result
- payment

# Estados do Dealer

- waiting
- setting-up-game
- 

# Estados do Player

- waiting
- chosing action
    - *Stand*(parar): Se o jogador está satisfeito com sua mão e não quer pedir mais cartas.
    - *Hit*(acertar): Se o jogador deseja mais uma carta
    - *Double*(dobrar)(OPCIONAL): Se o jogador sentir que necessita de uma e somente uma carta adicional, então pode dobrar sua aposta e receber mais uma carta, boa ou ruim. Esta opção é oferecida somente nas duas primeiras cartas, e às vezes nas duas primeiras cartas após *split*.
    - *Split*(dividir)(OPCIONAL): Se as primeiras duas cartas do jogador forem de mesmo valor em pontos, ele pode dividi-las em duas mãos. Neste envento cada carta é a primeira carta de uma mão nova. O jogador deve também fazer uma outra aposta de valor igual à primeira para a seguinda mão. O jogador pode geralmente dividir até duas ou três vezes consecutivas se aparecer a oportunidade. Dobrar após dividir pode ser permitido, mas nem sempre é o caso.
    - *Surrender*(rendição)(OPCIONAL): Alguns cassinos oferecem ao jogador a opção de rendição nas primeiras duas cartas. Se o jogador não gostar das duas cartas pode perder metade do valor da aposta assim como suas cartas. Esta opção geralmente é oferecida somente depois que o negociante verifica se há o blackjack.
- surrendered

