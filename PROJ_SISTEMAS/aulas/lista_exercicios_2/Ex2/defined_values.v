module defined_values
(
    output wire[7:0] a, b, output wire[4:0] c, output wire[31:0] d, e, f
);

// a deve receber o valor binario 1011 0101, sem sinal, com 8 bits
assign a = 8'b1011_0101;

// b deve receber o valor decimal 42, com sinal, com 8 bits
assign b = 8'sd42;

// c deve receber o valor hexadecimal 11, sem sinal, com 5 bits
assign c = 5'h11;

// d deve receber o valor decimal 4, sem sinal, com 32 bits
assign d = 4'd4;

// e deve receber o valor octal -100, com sinal, com 32 bits
assign e = -32'so100;

// f deve receber o valor decimal 7, com sinal, com 32 bits
assign f = 32'sd7;

endmodule