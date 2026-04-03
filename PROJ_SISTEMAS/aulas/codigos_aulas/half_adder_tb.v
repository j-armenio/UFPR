`timescale 1ns/1ps

module half_adder_tb ();

    reg t_a, t_b;
    wire t_sum, t_carry_out;

    half_adder DUT (.a(t_a), .b(t_b),
                .sum(t_sum), .carry_out(t_carry_out));

initial begin
    $dumpfile("half_adder_tb.vcd");
    $dumpvars(0, half_adder_tb);
end

initial begin
    t_a = 1'b0; t_b = 1'b0;
#10 t_a = 1'b0; t_b = 1'b1;
#10 t_a = 1'b1; t_b = 1'b0;
#10 t_a = 1'b1; t_b = 1'b1;

#10 $finish();
end

endmodule