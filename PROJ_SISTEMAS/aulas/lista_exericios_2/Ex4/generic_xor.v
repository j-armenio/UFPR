module generic_xor
#(
    parameter NUM_BITS=8
)
(
    input wire[NUM_BITS-1:0] a,
    input wire[NUM_BITS-1:0] b,
    output wire[NUM_BITS-1:0] c
);

    assign c = a ^ b;

endmodule