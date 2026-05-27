module pong_logic (
    input wire clk,            // Clock de pixel
    input wire resetn,         // Reset ativo em 0
    input wire vsync_pulse,    // Pulso que indica o fim de um frame
    input wire btn_invert,     // Botão para inverter o paddle

    // Saídas de coordenadas para o renderizador
    // 12 bits para contar até 4095 (mais doq precisa)
    output reg [11:0] ball_x,  // Posicao horizontal (x) da bola
    output reg [11:0] ball_y,  // Posição vertical (y) da bola
    output reg [11:0] paddle_y // Posição vertical (y) do paddle
);
    
    // Variáveis de estado interno
    reg paddle_dir_up; // 1 = paddle subindo,         0 = descendo
    reg ball_dir_x;    // 1 = bola indo para direita, 0 = esquerda
    reg ball_dir_y;    // 1 = bola indo para baixo,   0 = para cima
    reg btn_prev;      // memoriza se o botão estava apertado no ciclo anterior

    // Constantes de configurações
    localparam SCREEN_W = 640;
    localparam SCREEN_H = 480;
    localparam BALL_SIZE = 10;
    localparam PADDLE_H = 60;
    localparam BALL_SPEED = 2;
    localparam PADDLE_SPEED = 2;
    
    always @(posedge clk or negedge resetn) begin

        // ===== Reset assíncrono =====
        if (!resetn) begin
            // Se o reset é 0, coloca todas variáveis em sua posição inicial
            ball_x <= SCREEN_W / 2;   // Coloca bola no meio da tela
            ball_y <= SCREEN_H / 2;   // Coloca bola no meio da tela
            paddle_y <= SCREEN_H / 2; // Coloca paddle no meio da tela
            paddle_dir_up <= 1'b0;    // Inicia paddle indo para baixo (descendo)
            ball_dir_x <= 1'b1;       // Inicia bola indo para direita
            ball_dir_y <= 1'b1;       // Inicia bola indo para baixo
            btn_prev <= 1'b0;         // Inicia memória do botão como solto
        
        // ===== Funcionamento normal =====
        end else begin
            // A cada ciclo de clock de pixel guarda o estado atual do botão na memória 'btn_prev'
            // Isso acontece milhões de vezes por segundo
            btn_prev <= btn_invert;
            
            // Mas a física do jogo não pode ser tão rápida
            // O 'if (vysnc_pulse)' garante que os cálculos só acontecem 1 vez por frame
            if (vsync_pulse) begin
                // Controle do paddle
                // O botão está apertado agora E não estava apertado antes
                if (btn_invert && !btn_prev) begin
                    paddle_dir_up <= ~paddle_dir_up;
                end

                // ===== Movimentação automática do paddle =====
                if (paddle_dir_up && paddle_y > 0)
                // Se está subindo E não bateu no teto, subtrai 2 pixels da posição y
                    paddle_y <= paddle_y - PADDLE_SPEED;
                else if (!paddle_dir_up && paddle_y < (SCREEN_H - PADDLE_H))
                // Se está descendo E não bateu no chão, soma 2 pixels na posição y
                    paddle_y <= paddle_y + PADDLE_SPEED;
                
                // ===== Física da bola =====
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
                
                // Verifica se a bola bateu no chão
                if (ball_y >= SCREEN_H - BALL_SIZE)
                    ball_dir_y <= 0; // Inverte direção vertical para cima
                // Verifica se a bola bateu no teto
                else if (ball_y <= 0) 
                    ball_dir_y <= 1; // Inverte direção vertical para baixo
            end
        end
    end
endmodule