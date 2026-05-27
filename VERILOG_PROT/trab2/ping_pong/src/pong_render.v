module pong_render (
    input wire clk,             // Clock de pixel
    input wire resetn,          // Reset ativo em 0
    input wire [11:0] pixel_x,  // Recebe a coluna do pixel atual sendo desenhado
    input wire [11:0] pixel_y,  // Recebe a linha do pixel atual sendo desenhado
    input wire video_active,    // 1 = Está na área visível da tela

    input wire [11:0] ball_x,   // Coordenada X atual da bola
    input wire [11:0] ball_y,   // Coordenada Y atual da bola
    input wire [11:0] paddle_y, // Coordenada Y atual do paddle esquerdo

    // Saída de cor para módulo HDMI, 24 bits divididos em 3 canais de 8 bits
    output wire [23:0] rgb_out
);

    // ===== Parâmetros de tamanho =====
    localparam BALL_SIZE = 10;
    localparam PADDLE_W = 15;
    localparam PADDLE_H = 60;    
    localparam PADDLE_X = 20; // O paddle esquerdo fica fixo na coluna(x) 20 da tela

    // ===== Verficação de colisões =====
    // Verifica se a coordenada sendo desenhada agora está dentro do quadrado da bola
    // 1 = true, 0 = false
    wire is_ball = (pixel_x >= ball_x) && (pixel_x <= ball_x + BALL_SIZE) &&
                   (pixel_y >= ball_y) && (pixel_y <= ball_y + BALL_SIZE);

    // Verifica se a coordenada sendo desenhada agora está dentro do quadrado do paddle
    wire is_paddle = (pixel_x >= PADDLE_X) && (pixel_x <= PADDLE_X + PADDLE_W) &&
                     (pixel_y >= paddle_y) && (pixel_y <= paddle_y + PADDLE_H);

    // ===== Seletor de cores =====
    wire [23:0] color;
    
    assign color = is_ball   ? 24'hFFFFFF : // Branco (bola)
                   is_paddle ? 24'h00FF00 : // Verde (paddle)
                               24'h000000 ; // Preto (fundo)

    // ===== Ativa máscara de vídeo =====
    // Só passa a cor se o 'video_active' for 1, se não passa preto.
    assign rgb_out = video_active ? color : 24'h000000;

endmodule