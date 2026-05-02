module mux_2(
  output wire R0, R1,
  input wire a0, a1, b0, b1,
  input wire select
);
  
  wire not_s;
  wire tmp_a0, tmp_b0;
  wire tmp_a1, tmp_b1;
  
  not(not_s, select);
  
  and(tmp_a0, not_s, a0);
  and(tmp_b0, select, b0);
  or(R0, tmp_a0, tmp_b0);
  
  and(tmp_a1, not_s, a1);
  and(tmp_b1, select, b1);
  or(R1, tmp_a1, tmp_b1);
  
endmodule