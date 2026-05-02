# Verilog Rush

Verilog é uma **Linguagem de Descrição de Hardware (HDL)**, você descreve como os fios e portas lógicas devem se conectar.

## 1. Tipos de dados e estrutura básica
Um projeto em Verilog é composto por **módulos**, caixas com entradas e saídas.
- `wire` : representa uma conexão física (um fio). Não armazena valor.
- `reg`: representa uma variável que mantém valor. Nem todo `reg` vira um registrador físico (flip-flop).
```verilog
module porta_and (
	input wire a, b,  // entradas
	output wire y     // saídas
);
	assign y = a & b; // atribuição contínua
endmodule
```
`wire` só aceita atribuição com `assign`.

## 2. Operadores e condicionais

### Operadores

| Tipos        |                   Operadores                    |
| :----------- | :---------------------------------------------: |
| Aritméticos  |            `+`,`-`,`*`,`/`,`%`,`**`,            |
| Lógicos      |         `&&`(AND), \|\| (OR), `!`(NOT)          |
| Bitwise      |                `&`,`\|`,`^`,`~`,                |
| Relacionais  |           `==`,`!=`,`>`,`<`,`>=`,`<=`           |
| Concatenação | `{a, b}` (junta bits de a e b em um barramento) |

### Condicionais (if/else e case)
Devem estar dentro de blocos de procedimento (`always`).
```verilog
always @(*) begin
	if (sel)
		y = a;
	else
		y = b;
end

// ou usando case (melhor para decodificadores)
always @(*) begin
	case (operacao)
		2'b00: resultado = a + b;
		2'b01: resultado = a - b;
		default: resultado = 0;
	endcase
end 
```
## 3. Lógica Combinacional x Sequencial 
### Lógica Combinacional (atribuição bloqueante `=`)
O valor da saída depende apenas das entradas atuais. Usamos `assign` ou `always @(*)`.
- Use `=` para lógica combinacional.

### Lógica Sequencial (atribuição não-bloqueante `<=`)
O valor depende de um estado anterior e de um sinal de sincronismo (clock).
- use `<=` para lógica sequencial em blocos `always @(posedge clk)`
```verilog
// Sequencial: contador simples
reg [3:0] count;
always @(posedge clk) begin
	if (reset)
		count <= 4'b0000;
	else 
		count <= count + 1;
end
```
Misturar `=` e `<=` no mesmo bloco `always` causa comportamento imprevisível.

## 4. Máquina de Estados (FSM)
São o "cérebro" do controle digital, divididas em três partes: **Lógica de Próximo Estado**, **Registrador de Estado** e **Lógica de Saída**.
```verilog
parameter S0 = 0, S1 = 1;
reg state, next_state;

// 1. Transição de estado (sequencial)
always @(posedge clk) begin
	if (reset) state <= S0
	else state <= next_state;
end

// 2. Lógica de próximo estado (combinacional)
always @(*) begin
	case (state)
		S0: next_stage = (in) ? S1 : S0;
		S1: next_stage = S0;
	endcase
end
```

## 5. Verificação e Testbench
É um código em Verilog que não vira hardware, serve para estimular um módulo e ver se ele funciona.

Elementos de simulação:
- `#delay`: atrasa a execução (ex: `#10` espera 10 unidades de tempo).
- `$display`: imprime mensagens no console (como o `printf`).
- `initial`: bloco que executa apenas uma vez no início da simulação.
```verilog
module tb_porta_and();
	reg t_a, t_b;
	wire t_y;
	
	// instancia o módulo que queremos testar (DUT)
	porta_and dut (.a(t_a), .b(t_b), .y(t_y));
	
	initial begin
		t_a = 0; t_b = 0; #10;
		t_a = 1; t_b = 0; #10;
		$display("Saída para A=1, B=0 eh %b", t_y);
		$finish; // encerra simulação
	end
endmodule
```

## Outros detalhes
- Hardware é paralelo: tudo que está fora de blocos `always` ou em blocos `always` diferentes acontece ao mesmo tempo.
- Sincronismo: use sempre `posedge clk` para circuitos complexos.