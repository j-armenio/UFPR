# Questão 1

A primeira era sobre Corrida. Tinha Corrida, CorridaDeRua e
CorridadeDeObstaculo e tinha que relacionar os tres. CorridaDeRua e
CorridaDeObstaculo tinham os mesmos dois métodos: 'inscreverParticipante(String
nome)' e 'entregarKit(String nome)'. Nesse ponto, era pra escolher entre usar
interface ou classe abstrata e fazer as mudanças para ficar correto. Depois,
tinha que implementar um classe chamada 'gerenciarCorrida' com um método para
inscrever e entregar o kit. No entanto, ele era polimórfico. Então, ele recebia
Corrida como parâmetro e tinha que fazer o downcast para instanciá-lo. Sim, ela
perguntou qual tipo usou. Depois (sim, a questão era longa), tinha que criar
uma main com um ArrayList com 2 objetos: CorridaDeRua e CorridaDeObstaculo.
Sim, denovo ela perguntou qual polimorfismo foi usado. Nesse caso, foi usado
polimorfismo Universal de Inclusão e polimorfimos Universal Paramêtrico.

# Questão 2

Tinha uma estrutura em UML de Hotel e tinha que implementar a classe HotelLuxo
com o construtor completo e métodos get/set. Só que na verdade o atributo era
um boolean, então era pra implementar os métodos is/set. Depois, ela perguntou
qual Design Pattern seria bom usar nesse caso. Como era um programa para criar
hospedagens, factory method foi o que eu coloquei e tinha que justificar
também.

```java

public boolean isLigado() {
    return this.ligado;
}

private void setLigado(boolean ligado) {
    this.ligado = ligado;
}

```

# Questão 3

Essa tinha várias respostas. A questão dava um contexto e a gente tinha que
escrever o UML dela. O contexto era sobre um projeto da SBC que tinha vários
projetos parceiros. Eu fiz uma classe abstrata ProjetoParceiro, a interface
Cadastro (para realizar o cadastro) e a interface Relatorio (para enviar o
relatório) com os métodos concretos sendo os exemplos de projetos parceiros, ou
seja, classes que herdavem ProjetoParceiro.

