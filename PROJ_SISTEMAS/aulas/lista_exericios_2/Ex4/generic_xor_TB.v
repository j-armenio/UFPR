module generic_xor_TB();
 
  reg[7:0] a_8, b_8;
  wire[7:0] c_8;

  reg[3:0] a_4, b_4;
  wire[3:0] c_4;

  generic_xor #(.NUM_BITS(8)) DUT8 (.a(a_8), .b(b_8), .c(c_8));
  generic_xor #(.NUM_BITS(4)) DUT4 (.a(a_4), .b(b_4), .c(c_4));
  initial begin
    a_8 = 8'b0000_0000;
    b_8 = 8'b1111_1111;
    #1
    if (c_8 != 8'b1111_1111) begin
        $display("Teste 1 - Falha em teste com 8 bits!");
        $display("Esperado: %b -- Resposta: %b", 8'b1111_1111, c_8);
    end
    else $display("Teste 1 - OK");

    a_8 = 8'b0101_0101;
    b_8 = 8'b1111_1111;
    #1
    if (c_8 != 8'b1010_1010) begin
        $display("Teste 2 - Falha em teste com 8 bits!");
        $display("Esperado: %b -- Resposta: %b", 8'b1010_1010, c_8);
    end
    else $display("Teste 2 - OK");

    a_4 = 4'b0101;
    b_4 = 4'b1110;
    #1
    if (c_4 != 4'b1011) begin
        $display("Teste 3 - Falha em teste com 4 bits!");
        $display("Esperado: %b -- Resposta: %b", 4'b1011, c_8);
    end
    else $display("Teste 3 - OK");

  end

endmodule