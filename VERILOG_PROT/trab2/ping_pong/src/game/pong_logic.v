module pong_logic (
    input wire clk,            // Clock de pixel
    input wire resetn,         // Reset ativo em 0
    input wire vsync_pulse,    // Pulso que indica o fim de um frame

    input wire btn1_pulse,     // Inverte jogador 1 (esquerda)
    input wire btn2_pulse,     // Inverte jogador 2 (direita)

    // Saídas de coordenadas para o renderizador
    // 12 bits para contar até 4095 (mais doq precisa)
    output reg [11:0] ball_x,   // Posicao horizontal (x) da bola
    output reg [11:0] ball_y,   // Posição vertical (y) da bola
    output reg [11:0] paddle1_y, // Posição vertical (y) do paddle 1
    output reg [11:0] paddle2_y // Posição vertical (y) do paddle 2
);

    // Variáveis de estado interno
    reg p1_dir_down, p2_dir_down; // 1 = paddle subindo,         0 = descendo
    reg ball_dir_x;               // 1 = bola indo para direita, 0 = esquerda
    reg ball_dir_y;               // 1 = bola indo para baixo,   0 = para cima

    // Constantes de configurações
    localparam SCREEN_W = 640;
    localparam SCREEN_H = 480;
    localparam BALL_SIZE = 10;
    localparam PADDLE_H = 60;
    localparam PADDLE_W = 15;

    // Posições fixas no eixo X dos jogadores
    localparam PADDLE1_X = 20;
    localparam PADDLE2_X = 605;

    localparam BALL_SPEED = 2;
    localparam PADDLE_SPEED = 2;
    
    always @(posedge clk or negedge resetn) begin

        // ===== Reset assíncrono =====
        if (!resetn) begin
            // Se o reset é 0, coloca todas variáveis em sua posição inicial
            ball_x <= SCREEN_W / 2;    // Coloca bola no meio da tela
            ball_y <= SCREEN_H / 2;
            paddle1_y <= SCREEN_H / 2; // Coloca os paddles no meio da tela
            paddle2_y <= SCREEN_H / 2;

            p1_dir_down <= 1'b1;       // p1 começa descendo
            p2_dir_down <= 1'b0;       // p2 começa subindo
            ball_dir_x <= 1'b1;        // Inicia bola indo para direita
            ball_dir_y <= 1'b1;        // Inicia bola indo para baixo
        
        // ===== Funcionamento normal =====
        end else begin
            // Leitura dos botões
            if (btn1_pulse) p1_dir_down <= ~p1_dir_down;
            if (btn2_pulse) p2_dir_down <= ~p2_dir_down;

            // Mas a física do jogo não pode ser tão rápida
            // O 'if (vysnc_pulse)' garante que os cálculos só acontecem 1 vez por frame
            if (vsync_pulse) begin
                // ===== Movimento do paddle 1 =====
                if (p1_dir_down && paddle1_y < (SCREEN_H - PADDLE_H))
                    paddle1_y <= paddle1_y + PADDLE_SPEED;
                else if (!p1_dir_down && paddle1_y > 0)
                    paddle1_y <= paddle1_y - PADDLE_SPEED;

                // ===== Movimento do paddle 2 =====
                if (p2_dir_down && paddle2_y < (SCREEN_H - PADDLE_H))
                    paddle2_y <= paddle2_y + PADDLE_SPEED;
                else if (!p2_dir_down && paddle2_y > 0)
                    paddle2_y <= paddle2_y - PADDLE_SPEED;

                // ===== Movimento da bola =====
                // Eixo X
                if (ball_dir_x)
                // Se está indo para direita, soma 2 pixels em x
                    ball_x <= ball_x + BALL_SPEED;
                else
                // Se está indo para esquerda, subtrai 2 pixels em x
                    ball_x <= ball_x - BALL_SPEED;
                
                // Eixo Y
                if (ball_dir_y)
                // Se está indo para baixo, soma 2 pixels em y
                    ball_y <= ball_y + BALL_SPEED;
                else
                // Se está indo para cima, subtrai 2 pixels em y
                    ball_y <= ball_y - BALL_SPEED;

                // ===== Movimento e Colisão Y da Bola =====
                if (ball_dir_y) begin // Se está indo para baixo
                    if (ball_y >= (SCREEN_H - BALL_SIZE - BALL_SPEED)) begin
                        ball_dir_y <= 0; // Previu que vai bater no chão: inverte
                    end else begin
                        ball_y <= ball_y + BALL_SPEED;
                    end
                end else begin        // Se está indo para cima
                    if (ball_y <= BALL_SPEED) begin
                        ball_dir_y <= 1; // Previu que vai bater no teto: inverte
                    end else begin
                        ball_y <= ball_y - BALL_SPEED;
                    end
                end

                // Colisão: Jogador 1 (esquerda)
                if (ball_x <= PADDLE1_X + PADDLE_W && ball_x + BALL_SIZE >= PADDLE1_X &&
                    ball_y + BALL_SIZE >= paddle1_y && ball_y <= paddle1_y + PADDLE_H) begin
                    ball_dir_x <= 1; // Rebate para direita
                end

                // Colisão: Jogador 2 (direita)
                else if (ball_x <= PADDLE2_X + PADDLE_W && ball_x + BALL_SIZE >= PADDLE2_X &&
                    ball_y + BALL_SIZE >= paddle2_y && ball_y <= paddle2_y + PADDLE_H) begin
                    ball_dir_x <= 0; // Rebate para esquerda
                end

                // Pontuação
                else if (ball_x >= SCREEN_W - BALL_SIZE || ball_x <= 0) begin
                    // Alguém tomou gol
                    ball_x <= SCREEN_W / 2;
                    ball_y <= SCREEN_H / 2;
                end
            end
        end
    end
    
endmodule
