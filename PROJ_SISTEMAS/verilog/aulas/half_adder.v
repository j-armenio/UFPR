module half_adder 
(
input wire a, b,
output wire sum, carry_out
);

    xor u1(sum, a, b);
    and u2(carry_out, a, b);

endmodule