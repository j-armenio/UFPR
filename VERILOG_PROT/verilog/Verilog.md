# 1. Aspectos gerais da Linguagem

É uma linguagem de descrição de Hardware.

1. **Múltiplos níveis de hierarquia**
![[Pasted image 20260402183157.png|565]]

2. **Descritiva**
```verilog
// Comportamental
if (a == b)
	equals <= 1;
else
	equals <= 0;
	
// Expressões lógicas
equals = (a == b) ? 1 : 0;

// Rede de Ligação (Estrutural)
xnor U0 (x[0], a[0], b[0]);
xnor U1 (x[1], a[1], b[1]);
``` 

3. **Concorrente**
Por padrão, comandos ocorrem de forma concorrente, são executados juntos.
```verilog
module portas(
	input wire a, b, c,
	output wire x, y);
	
	assign x = a & b;
	assign y = c | b;
	
endmodule
```
Esse código gera a seguinte arquitetura:
![[Pasted image 20260402184259.png|339]]

4. **Comandos Sequenciais**
É possível definir que comandos tenham uma execução sequencial. Dentro do bloco `always` ou `initial`, o comportamento depende do tipo de atribuição que você usa ( **=** atribuição bloqueante, **<=** comportamento concorrente).
![[Pasted image 20260402185309.png|383]]

# 2. Síntese de Circuitos

## 2.1. Fluxo de desenvolvimento
![[Pasted image 20260402190749.png|474]]

## 2.2. Síntese
### Compilação/elaboração
Código HDL é analisado e convertido em um conjunto de elementos como portas and, or, somadores, memórias, flip-flops e multiplexadores.

### Otimização
Avalia o design em busca de:
- reduzir número de portas lógicas
- reduzir a complexidade do roteamento
- ...
A compilação do código Verilog é independente ao FPGA utilizado.

A otimização de mapeamento de tecnologia visa utilizar elementos específicos mais complexos do FPGA selecionado para aprimorar o resultado da compilação.

## 2.3. Projeto top-down
Um projeto pode ser refinado de mais de uma forma, em várias fases. Na sua fase inicial, e a cada refinamento, o resultado é mais concreto e próximo da implementação.

Partindo-se de uma **especificação** (abstrata) do comportamento desejado, esta sofre um refinamento que resulta num **diagrama de blocos**, que ao ser refinado produz o **projeto dos blocos**. O refinamento dos blocos resulta na **formalização da especificação**, que é a **modelagem concreta dos componentes** do projeto em Verilog (código).
# 3. Softwares de prototipação

• **DigitalJS (Web ou VSCode)** 
Visualizador que converte código em um diagrama de circuito digital.

• **Gowin EDA Education**  
Software para FPGAs da Gowin.

• **IcarusVerilog** 
Compilador e simulador via CLI.
# 4. Estrutura do Código Verilog

Possui uma **Hierarquia de módulos**.
![[Pasted image 20260402194452.png|583]]

**Nível Estrutural**
- Descreve o hardware instanciando componentes e conectando-os com fios. É a representação mais fiel ao hardware físico.
- Usa primitivas e outros módulos que já criou. 
- Possível especificar a temporização dos componentes.
- Maior tempo de programação e (possível) maior desempenho.
```verilog
module and_gate
(
	input wire a, b,
	output wire z
);

	// parâmetros
	// sinais internos
	// ips
	// ...
	and(z, a, b);
	
endmodule
```
**Nível Comportamental**
- Descreve o algoritmo ou a função lógica do circuito.
- A ferramenta de prototipação gera automaticamente o melhor circuito que ela conseguir.
- Menor tempo de programação e (possível) menor desempenho. 
```verilog
module somador
(
	input wire[3:0] a, b,
	output wire[3:0] z
);

	// ...
	assign z = a + b;

endmodule
```
### Portas de um módulo
```verilog
module example
(
	input tipo_1 a, b; // entradas
	output tip_2 x, y; // saídas
		// pode ser lido dentro do código
	inout tipo_4 z1, z2; // entrada/saída
)
```
![[Pasted image 20260402202818.png|308]]

### Funcionamento
```verilog
module exemplo (...);
	// declarações:
		// sinais, constantes, tarefas, funções, outros

// entrada/saída
endmodule
```
## 4.1. Instaciação de módulos
A nível estrutural, é possível aproveitar módulos já existentes para construir novos.
- No exemplo, `mux_4` usa `mux_2`.
```verilog
module mux_2
(
	output wire out,
	input wire e1, e2,
	input wire s
);

module mux_4
(
	input wire a, b, c, d,
	input wire[1:0] selector,
	output wire z
);
	wire tmp1, tmp2;
	mux_2 u1(tmp1, a, b, selector[0]); 
	mux_2 u2(tmp2, c, d, selector[0]); // parâmetros podem ser passados em ordem
	// ou como referência
	// mux_2(.e1(a), .e2(b), .out(tmp1), .s(selector[0]))
	mux_2 u3(z, tmp1, tmp2, selector[1]);
endmodule
```

## 4.2. Primitivas a nível de portas
Verilog disponibiliza algumas primitivas (funções) simples: **not**, **and**, **nand**, **or**, **nor**, **xor**, **xnor**.
- Primeiro argumento é usado como saída.
- Aceitam qualquer número de entradas, com exceção do **not** (apenas uma entrada).
```verilog
not(R, a);
and(R, a, b, c, d, e);
nor(R, a, d);
```
# 5. Testbench

![[Pasted image 20260402205226.png|397]]
Serve para depuração de códigos aplicados em síntese.
- Simulação funcional e temporal.
- Código não-sintetizável.
- É uma entidade sem sinais externos.

**Icarus Verilog** é um compilador, permite a simulação de aplicações Verilog. 
**GTKWave** permite observar o comportamento e realizar avaliações dos códigos.

Compile com: 
`>> iverilog -o nome modulo.v testbench.v`
Execute com: 
`>> vvp example`
Se o testbench gerar um arquivo VCD, é possível utilizar o GTKWave. 
`>> gtkwave example.vcd`

Exemplo de testbench usando DUT:
- Módulo half_adder
```verilog
module half_adder
(
input wire a, b,
output wire sum, carry_out
);

	xor u1(sum, a, b);
	and u2(carry_out, a, b);
	
endmodule
```
- Testbench
```verilog
`timescale 1ns/1ps

module half_adder_tb();

	reg t_a, t_b;
	wire t_sum, t_carry_out;
	
	half_adder DUT (.a(t_a), .b(t_b),
					.sum(t_sum), .carry_out(t_carry_out));

initial begin
	t_a = 1'b0; t_b = 1'b0;
#10 t_a = 1'b0; t_b = 1'b1;
#10 t_a = 1'b1; t_b = 1'b0;
#10 t_a = 1'b1; t_b = 1'b1;
// atrasos não são sintetizaveis
end
``` 
# 6. AAAA
