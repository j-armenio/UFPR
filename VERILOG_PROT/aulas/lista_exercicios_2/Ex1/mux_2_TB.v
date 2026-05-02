`timescale 1ns/1ps
module mux_2_TB();

wire R0, R1;
reg a0, a1, b0, b1, select;

mux_2 DUT(.a0(a0), .a1(a1), .b0(b0), .b1(b1), .select(select), .R0(R0), .R1(R1));

initial begin
  $dumpfile("mux_2.vcd");
  $dumpvars(0, mux_2_TB);
end

initial begin
  a0=0; a1=0; b0=1; b1=1;
  select = 0;
  #10
  select = 1;
  #10
  a0=1; a1=0; b0=0; b1=1;
  #10
  select = 0;
  #10
  $finish();

end
endmodule