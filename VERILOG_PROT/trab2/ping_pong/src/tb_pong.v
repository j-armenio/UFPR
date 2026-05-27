// Cada 1 unidade de tempo equivale a 1ns e o simulador vai calcular frações de
// tempo com precisão de 1ps
`timescale 1ns/1ps

module tb_pong;

    // ===== Estímulos =====
    reg clk;
    reg resetn;
    reg btn_invert;

    // Contadores virtuais que vão simular o comportamento HDMI
    reg [11:0] sim_pixel_x;
    reg [11:0] sim_pixel_y;
    reg sim_vsync_pulse;
    reg sim_video_active;

    // ===== Sinais de observação =====
    wire [11:0] ball_x, ball_y, paddle_y;
    wire [23:0] rgb_out;

    // ===== DUTs =====
    pong_logic u_logic (
        .clk(clk),
        .resetn(resetn),
        .vsync_pulse(sim_vsync_pulse),
        .btn_invert(btn_invert),
        .ball_x(ball_x),
        .ball_y(ball_y),
        .paddle_y(paddle_y)
    );

    pong_render u_render (
        .clk(clk),
        .resetn(resetn),
        .pixel_x(sim_pixel_x),
        .pixel_y(sim_pixel_y),
        .video_active(sim_video_active),
        .ball_x(ball_x),
        .ball_y(ball_y),
        .paddle_y(paddle_y),
        .rgb_out(rgb_out)
    );

    // ===== Gera clock virtual =====
    initial clk = 0;

    // A cada 20ns muda o clock
    // Um ciclo completo (0 -> 1 -> 0) dura 40ns = 25MHz (clock típico HDMI)
    always #20 clk = ~clk;

    // ===== Bloco de teste =====
    initial begin
        $dumpfile("sim_pong.vcd");
        $dumpvars(0, tb_pong);

        // Inicialização
        resetn = 0; 
        btn_invert = 0;
        sim_pixel_x = 0; 
        sim_pixel_y = 0; 
        sim_vsync_pulse = 0; 
        sim_video_active = 1;

        // Depois de 100ns ativa o sistema
        #100 resetn = 1;

        // Simulação de HDMI
        repeat (10) begin
            // Simula o desenho de um quadro de 640x480
            repeat (640 * 480) begin
                @(posedge clk);

                // Avança um pixel para direita
                sim_pixel_x = sim_pixel_x + 1;
                
                // Chega ao fim da linha, volta a esquerda e desce uma linha
                if (sim_pixel_x == 640) begin
                    sim_pixel_x = 0;
                    sim_pixel_y = sim_pixel_y + 1;
                    
                    if (sim_pixel_y == 480) begin
                        sim_pixel_y = 0;
                        sim_vsync_pulse = 1; // Avisa que passou 1 frame
                    end
                end else begin
                    // Garante que o pulso dura apenas 1 ciclo de clock
                    sim_vsync_pulse = 0;
                end
            end
        end

        // Simulação da interação do jogador
        // Simula pressionar botão físico
        btn_invert = 1;
        repeat (640 * 480) @(posedge clk);
        btn_invert = 0;

        repeat (5) begin
             repeat (640 * 480) begin
                @(posedge clk);
                // ... (mesma lógica de incremento de x, y e vsync) ...
                sim_pixel_x = sim_pixel_x + 1;
                if (sim_pixel_x == 640) begin
                    sim_pixel_x = 0;
                    sim_pixel_y = sim_pixel_y + 1;
                    if (sim_pixel_y == 480) begin
                        sim_pixel_y = 0;
                        sim_vsync_pulse = 1;
                    end
                end else sim_vsync_pulse = 0;
            end
        end

        $finish;
    end

endmodule