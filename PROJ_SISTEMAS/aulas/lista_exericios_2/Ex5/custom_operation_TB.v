module custom_operation_TB();
reg[7:0] a, b, c;
wire[3:0] d;

    custom_operation DUT (.a(a), .b(b), .c(c), .d(d));

    initial begin
        $monitor ("a: %b b: %b c: %b => d: %b", a, b, c, d);

        // Teste 1
        a = 42; b = 175; c = 93;
        #1 
        if (d == 4'b0110) $display("Teste 1 - OK");
        else begin
        $display("Teste 1 - Falha!");
        $display("Esperado: %b -- Resposta: %b", 4'b0110, d);
        end

        // Teste 2
        a = 53; b = 178; c = 1;
        #1 
        if (d == 4'b1111) $display("Teste 2 - OK");
        else begin
        $display("Teste 2 - Falha!");
        $display("Esperado: %b -- Resposta: %b", 4'b1111, d);
        end

        // Teste 3
        a = 14; b = 75; c = 254;
        #1 
        if (d == 4'b1111) $display("Teste 3- OK");
        else begin
        $display("Teste 3 - Falha!");
        $display("Esperado: %b -- Resposta: %b", 4'b1111, d);
        end
    end

endmodule