module top (
    input wire        clk,    // Clock 27MHz
    input wire        btn1_n, // Botão físico 1
    input wire        btn2_n, // Botão físico 2
    output wire [5:0] led,    // Leds

    // Fios de saída para conector HDMI físico
    output wire       tmds_clk_n,
    output wire       tmds_clk_p,
    output wire [2:0] tmds_d_n,
    output wire [2:0] tmds_d_p
);

    wire clk_p5;   // Clock de alta velocidade (125MHz)
    wire clk_p;    // Clock estável de pixel (25MHz)
    wire pll_lock; // Fio que sinaliza quando clocks se estabilizam

    Gowin_rPLL u_pll (
        .clkin(clk),
        .clkout(clk_p5),
        .lock(pll_lock)
    );

    Gowin_CLKDIV u_div_5 (
        .clkout(clk_p),
        .hclkin(clk_p5),
        .resetn(pll_lock)
    );

    // ===== Conexão com Camada de Controle =====
    wire btn1_pulse;
    wire btn2_pulse;
    wire game_resetn;

    // Controlador de entrada deve rodar em síncronia com o de vídeo
    input_controller u_input (
        .clk(clk_p),
        .btn1_in(~btn1_n), // Inverte lógica para positiva (1 = apertado)
        .btn2_in(~btn2_n),
        .btn1_pulse(btn1_pulse),
        .btn2_pulse(btn2_pulse),
        .game_resetn(game_resetn)
    );

    // ===== Conexão com Camada de Jogo =====
    wire [11:0] pixel_x;
    wire [11:0] pixel_y;
    wire        video_active;
    wire        vsync_pulse;
    wire [23:0] game_rgb;

    wire [11:0] ball_x;
    wire [11:0] ball_y;
    wire [11:0] paddle1_y;
    wire [11:0] paddle2_y;

    wire [2:0] p1_score; // Carrega pontuação
    wire [2:0] p2_score;

    pong_logic u_game_logic (
        .clk(clk_p),
        .resetn(game_resetn),
        .vsync_pulse(vsync_pulse),
        .btn1_pulse(btn1_pulse),
        .btn2_pulse(btn2_pulse),
        .ball_x(ball_x),
        .ball_y(ball_y),
        .paddle1_y(paddle1_y),
        .paddle2_y(paddle2_y),
        .p1_score(p1_score),
        .p2_score(p2_score)
    );

    pong_render u_game_render (
        .clk(clk_p),
        .resetn(game_resetn),
        .pixel_x(pixel_x),
        .pixel_y(pixel_y),
        .video_active(video_active),
        .ball_x(ball_x),
        .ball_y(ball_y),
        .paddle1_y(paddle1_y),
        .paddle2_y(paddle2_y),
        .p1_score(p1_score),
        .p2_score(p2_score),
        .rgb_out(game_rgb)
    );

    // ===== Conexão do Controlador HDMI ====
    svo_hdmi u_hdmi (
        .clk(clk),
        .resetn(pll_lock),
        .clk_pixel(clk_p),
        .clk_5x_pixel(clk_p5),
        .locked(pll_lock),
        
        // Links de controle que alimentam o renderizador e o loop do jogo
        .out_pixel_x(pixel_x),
        .out_pixel_y(pixel_y),
        .out_vsync_pulse(vsync_pulse),
        .out_video_active(video_active),
        .rgb_in(game_rgb), // Envia a cor do Pong direto pro cabo HDMI

        // Saídas de hardware físicas
        .tmds_clk_n(tmds_clk_n),
        .tmds_clk_p(tmds_clk_p),
        .tmds_d_n(tmds_d_n),
        .tmds_d_p(tmds_d_p)
    );

    // --- 5. INTERRUPTORES DE MONITORAMENTO (LEDs) ---
    // Mantemos os LEDs funcionando como seu painel de instrumentos!
    reg led1_state = 1'b0;
    reg led2_state = 1'b0;
    always @(posedge clk_p) begin
        if (btn1_pulse) led1_state <= ~led1_state;
        if (btn2_pulse) led2_state <= ~led2_state;
    end

    assign led[5] = ~led1_state;
    assign led[0] = ~led2_state;
    assign led[4] = game_resetn;
    assign led[3] = game_resetn;
    assign led[2] = game_resetn;
    assign led[1] = game_resetn;


endmodule
