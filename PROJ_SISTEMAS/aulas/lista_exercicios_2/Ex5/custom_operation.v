module custom_operation
(
    input wire[7:0] a, b, c,
    output wire[3:0] d
);

    wire[7:0] valores[2:0];

    assign valores[0] = a;
    assign valores[1] = b;
    assign valores[2] = c;

    assign d[0:0] = valores[][]

endmodule