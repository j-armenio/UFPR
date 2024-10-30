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