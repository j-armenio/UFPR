module defined_values_TB();
wire[7:0] a, b;
wire[4:0] c;
wire[31:0] d, e, f;

defined_values DUT(.a(a), .b(b), .c(c), .d(d), .e(e), .f(f));


initial begin
// a deve receber o valor binario 1011 0101, sem sinal, com 8 bits
$write("a -> ");
if (a == 181) $display("OK"); else $display("ERRO");

// b deve receber o valor decimal 42, com sinal, com 8 bits
$write("b -> ");
if (b == 42) $display("OK"); else $display("ERRO");

// c deve receber o valor hexadecimal 11, sem sinal, com 4 bits
$write("c -> ");
if (c == 17) $display("OK"); else $display("ERRO");

// d deve receber o valor decimal 4, sem sinal, com 32 bits
$write("d -> ");
if (d == 4) $display("OK"); else $display("ERRO");

// e deve receber o valor octal -100, com sinal, com 32 bits
$write("e -> ");
if (e == -64) $display("OK"); else $display("ERRO");

// f deve receber o valor decimal 7, com sinal, com 32 bits
$write("f -> ");
if (f == 7) $display("OK"); else $display("ERRO");

end
endmodule