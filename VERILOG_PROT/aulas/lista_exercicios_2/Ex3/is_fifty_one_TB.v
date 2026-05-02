module is_fifty_one_TB();
  reg[5:0] bits;
  wire is_equal;
  reg result_value;
  

  is_fifty_one DUT(.is_equal(is_equal), .bit_0(bits[0]), .bit_1(bits[1]), .bit_2(bits[2]), .bit_3(bits[3]), .bit_4(bits[4]), .bit_5(bits[5]));

  always @*
    result_value = is_equal;

  initial begin : testes
    bits = 0;
    
    for (integer i=0; i < 7'b1000000; i++)
    begin
      #1
      if ((bits == 51 && result_value != 1) || (bits != 51 && result_value != 0) || result_value === 1'bX) begin
        $display("ERRO => Valor: %d retornou %b", bits, result_value);
        disable testes;
      end
      
      bits = bits + 1;
    end

  $display("Componente correto!");

  end

endmodule