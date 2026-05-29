module input_controller (
    input wire clk, // clock de pixel (25MHz)

    // Entrada que vem dos botões físicos
    input wire btn1_in,
    input wire btn2_in,

    // Saídas para pong_logic
    // Emitem '1' durante UM ciclo de clock quando um botão é apertado
    output reg btn1_pulse,
    output reg btn2_pulse,

    // Sinal de reset (0 = reset) quando ambos botões são pressionados por 2 segundos
    output reg game_resetn 
);

    // Variáveis de estado interno

    // Contadores para medir por quanto tempo os botões estão pressionados
    // 16 bits contam até 65.535, a 25MHz, isso equivale a cerca de 2.6 milissegundos
    reg [15:0] db_cnt1, db_cnt2;

    // Guardam o estado final dos botões (1 = pressionado, 0 = solto)
    reg btn1_stable, btn2_stable;

    // Memória do ciclo de clock anterior para detectar quando o botão acabou de ser pressionado
    reg btn1_prev, btn2_prev;

    // Contador para medir 2 segundos para temporizador de reset
    // 26 bits conta até cerca de 67 milhões
    reg [26:0] reset_cnt;

    // Ciclos de clock que equivalem a 2 segundos
    // 2 seg * 25.000.000 Hz = 50.000.000
    localparam RESET_TIME = 50_000_000;

    always @(posedge clk) begin
        // ===== Debounce botão 1 =====
        if (btn1_in) begin
            // Se o botão físico for pressionado: aumenta o contador até atingir o máximo
            if (db_cnt1 < 16'hFFFF)
                db_cnt1 <= db_cnt1 + 1;
            else
                // Chegou ao máximo, botão estável
                btn_stable <= 1'b1;
        end else begin
            // Se o botão foi solto (ou trepidação), zeramos o contador
            db_cnt1 <= 0;
            btn1_stable <= 1'b0;
        end

        // ===== Debounce botão 2 =====
        if (btn2_in) begin
            // Se o botão físico for pressionado: aumenta o contador até atingir o máximo
            if (db_cnt2 < 16'hFFFF)
                db_cnt2 <= db_cnt2 + 1;
            else
                // Chegou ao máximo, botão estável
                btn_stable <= 1'b1;
        end else begin
            // Se o botão foi solto (ou trepidação), zeramos o contador
            db_cnt2 <= 0;
            btn1_stable <= 1'b0;
        end

        // Atualiza memória do estado anterior
        btn1_prev <= btn1_stable;
        btn2_prev <= btn2_stable;

        // O btn1_pulse só vai a '1' se:
        //   - O botão 1 acabou de estabilizar em 1 (btn1_stable)
        //   - Não estava em 1 anteriormente (!btn1_prev)
        btn1_pulse <= (btn1_stable && !btn1_prev);
        btn2_pulse <= (btn2_stable && !btn2_prev);

        // ===== Reset ao apertar 2 botões =====
        // Se ambos botões estão pressionados e estáveis
        if (btn1_stable && btn2_stable) begin
            // Verifica se o contador ainda não chegou a 2 segundos
            if (reset_cnt < RESET_TIME) begin
                reset_cnt <= reset_cnt + 1;
                game_resetn <= 1'b1; // Mantém o jogo rodando
            end else begin
                // Passaram 2 segundos com os botões pressionados
                game_resetn <= 1'b0;
            end
        end else begin
            // Se o jogador soltar algum dos botões, cancela a contagem e zera o contador
            reset_cnt <= 0;
            game_resetn <= 1'b1;
        end
    end

endmodule
