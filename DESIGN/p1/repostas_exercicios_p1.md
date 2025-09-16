# Exercícios P1 Design de Software

## 1
Universidade - Departamento
Relacionamento de Composição
1 - 1..*

Professor - Disciplina
Relacionamento de Associação
1 - 1..*
Agregação, disciplinas podem existir sem um professor específico

Aluno - Matrícula
Relacionamento de Composição
1 - 1..*

Curso - Pré-requisito
Relacionamento de Associação Simples
1 - 0..*
Associação, pois um curso pré-requisito existe independentemente de um Curso

## 2
Cliente -> Sistema, síncrona, adicionaItemCarrinho()
Cliente -> Sistema, síncrona, finalizaCompra()
Sistema -> Estoque, síncrona, verificaDisponibilidade()
Estoque -> Sistema, assíncrona, devolveDisponibilidade()
    (alt) Itens indisponíveis
    Sistema -> Cliente, assíncrona, notificaFaltaEstoque()
Sistema -> Pagamento, síncrona, processaPagamento()
Pagamento -> Sistema, assíncrona, confirmaPagamento()
    (alt) Pagamento falhou
    Sistema -> Cliente, assíncrona, notificaFalhaPagamento()
Sistema -> Entrega, síncrona, solicitaAgendamentoEntrega()
Entrega -> Sistema, assíncrona, confirmaAgendamento()
    (alt) Falha no agendamento
    Sistema -> Cliente, assíncrona, notificaFalhaAgendamento()
Sistema -> Cliente, assíncrona, notificaSucesso()

## 3
1. Passo 1, validação no formato do login. Entre passos 4 e 5, verificação de
expiração do token. Após passo, verificação de permissões do usuário. 

Validações de segurança devem ser feitas em todos passos onde algum tipo de 
autenticação é feita, ou seja, nos passos 2 e 6.

2. Implementar cache entre passos 3-4 e 9-10. Usar conexões persistentes com o
banco de dados. 

A Auth faz validação duas vezes, uma quando gera o token e outra quando o usuário 
pede dados para a API, essa segunda validação poderia ser removida pois
já foi executada uma vez.

3. Após o passo 2, caso o usuário não fosse validado, após o passo 6, caso o token
não fosse validado e após o passo 8, caso os dados não fossem encontrados pelo
Service.

4. Após o passo 4, logs de acesso. Atualizações de estatísticas após o passo 10.
Notificações secundárias.

## 4
1. Cliente chega e se registra via autoatendimento
3. Cliente registra seu problema
4. Sistema faz a triagem automática
5. Sistema resolve o problema
5.1. Sistema oferece retornos alternativos (chat, redirecionar a atendente)

## 5
1. Os componentes possuem alto acoplamento entre sí, logo se um componente com
que é dependência de vários componentes quebrar, o sistema como um todo poderá 
ficar indisponível.

2. É possível introduzir interfaces entre componentes, assim, um componente não
depende mais diretamente de outro, mas sim de um contrato bem definido.

3. Padrões com um baixo acomplamento são o de Microserviços e em Camadas, onde
caso um componente precise de manutençao/atualizações, ele pode ser facilmente
substituido sem a quebra do resto do sistema.

## 6 (?)
1. 
2. 
3. 
4. 

## 7 (?)
1. Serve para mostrar as relações entre os atores e as ações que eles podem
tomar dentro de um sistema.

2. Serve para mostrar a arquitetura estática de um sistema e as relações entre
seus componentes.

3. Serve para mostrar o fluxo temporal de ações em um sistema, o ordem a comunicação
ocorre e quem se comunica com quem.

4. Útil para deixar claro lógicas de processos, decisões e paralelismo.

5. Útil para demonstar o estado de um sistema ao longo de sequências de ações.

6. Útil para demonstrar o relacionamento estático entre componentes de um sistema.

7. Útil para demonstrar qual a infraestrutura e as tecnologias que compoẽm um sistema.

## 8
1. Microserviços. Apresenta grande escalabilidade e disponibilidade, tendo uma
boa tolerância a falhas, já que outros serviços podem ser facilmente subidos caso
um falhe.

2. Orientado a Eventos. Ocorre em tempo real, já que é chamado por eventos, tem
baixa latência e alta confiabilidade.  
Monolítico. Como precisa de uma baixa latência e confiabilidade, um sistema
sólido e unificado é necessário. 

3. Monolítico. (?)

4. Orientado a Eventos e Microserviços. É composto por diversos serviços, altamente
escalável e tem funcionamento direcionado por eventos como compras, que chamam
o sistema.

5. Microserviços. Possibilita uma modernização gradual, já que serviços podem
ser incluidos separadamente.

## 9.1
Monolítico: time pequeno que quer produzir um MVP(Produto Mínimo Viável)
* Simples de implementar inicialmente, consistência fácil.
* Difícil escalar, baixa confiabilidade (se algo quebra, tudo quebra).
Microserviços: aplicação que precisa suportar um enorme número de usuários, como
uma rede social. Sistemas grandes com múltiplas equipes.
* Alta escalabilidade granular, alta disponibilidade (se um serviço cair, só subir
outro).
* Complexa observabilidade, custo alto (infra, latência, coordenação, etc).
Event-Driven: aplicação que tem ativação dita por eventos, como uma plataforma de
e-commerce. Sistemas em tempo real, IoT.
* Resistente a picos de usuários, alta escalabilidade e desacoplamento. 
* Maior complexidade de debug e difícil garantir consistência de mensagens enviadas.
Camadas: aplicação web tradicional (front, back, banco).
* Boa manutenibilidade na troca de camadas, alta coesão.
* Pode ocorrer latência e sobrecarga entre camadas.
Pipeline: Aplicações ETL, processamento de dados
* Boa escalabilidade para pipelines de dados, simples de entender como fluxo de
transformações.
* Difícil observabilidade dos dados no meio do fluxo, difícil garantir consistência.

## 9.2 (?)
1. 
* NF- Desempenho
* NF- Desempenho
* NF- Disponibilidade
* NF- Segurança
* NF- Compatibilidade
* NF- Escalabilidade
* NF- Conformidade
* NF- Confiabilidade
* NF- Manutenibilidade
* NF- Observalidade

2 e 3. 
* 5.000 transações por minuto (desempenho), 
* tempo de resposta de máximo 500ms para 95% das requisições (desempenho), 
* disponibilidade de 99.99%, sistema deve suportar aumento de 300% de carga em 
períodos promocionais (disponibilidade), 
* deployment de novas funcionalidades sem dowtime (manutenibilidade),
* logs detalhados de todas operações críticas (observabilidade)

## 10
A. Sistema de Saúde (Prontuário Eletrônico)
- Conformidade HIPAA e LGPD (Conformidade)
- Disponibilidade 24/7 para emergências (Disponibilidade)
- Acesso auditado e rastreável (Observabilidade)
- Dados devem ser mantidos por 20 anos (Manutenibilidade)
- Interface intuitiva para profissionais médicos (Usabilidade)
Garantir conformidade para o serviço sacrifica agilidade no desenvolvimento do
projeto.

B. Rede Social (Plataforma de Vídeos)
- Suportar 1 milhão de usuários concorrentes (Escalabilidade)
- Tolerância a picos de tráfego (Escalabilidade)
- Baixa latência para streaming (Desempenho)
- Baixo custo de infraestrutura (Custo)
- Recomendações personalizadas em tempo real (Usabilidade)
A aplicação pede um baixo custo de infraestrutura, porém deve ter uma baixa
latência e suportar muitos usuários, ainda preparado para picos maiores, o que
torna difícil manter o custo baixo.

C. Sistema de Controle Industrial
- 100% de confiabilidade operacional (Confiabilidade)
- Tempo real com latẽncia até 10ms (Desempenho)
- Resistente a interferências eletromagnéticas (Segurança)
- Manutenção remota segura (Segurança)
- Funcionamento offline por 72 horas (Disponibilidade)
É difícil manter 100% de confiabilidade e manter o funcionamento offline.

## 11
Diagrama 1
Em Camadas
Divisão hierárquica de responsabilidades em camadas, dependência unidirecional.
Fácil manutenção porém pode ter latência entre camadas.
Aplicação Web tradicional, com interface do usuário frontend, lógicas de negócio
backend, camada de persistência de um banco de dados simples. Sistemas empresariais. 

Diagrama 2
Cliente-Servidor
Existe um provedor e um consumidor.
Dados centralizados porém contém ponto único de falha sem redundância.
Sistemas bancários, email

Diagrama 4
Microserviços
Diversos serviços independentes.
Alta escalabilidade porém alto custo.
Plataforma de E-commerce com compras e pagamentos. Plataformas de streaming.

Diagrama 5
Pipe & Filters
Fluxo claro de processamento atráves de filtros com transformações.
Alta modularidade e reutilização porém dificil observabilidade dos dados.
Aplicação de ETL, compiladores, processamento de imagens.

Diagrama 6
Orientado a Eventos
Distribuição de mensagens atráves de eventos gerados por um produtor de eventos.
Alta escalabilidade e desacoplamento porém debugging complexo.
Serviço de assinatura de notícias, sistemas de notificação e IoT.
