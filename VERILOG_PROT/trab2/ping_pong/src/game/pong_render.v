module pong_render (
    input wire clk,              // Clock de pixel
    input wire resetn,           // Reset ativo em 0
    input wire [11:0] pixel_x,   // Recebe a coluna do pixel atual sendo desenhado
    input wire [11:0] pixel_y,   // Recebe a linha do pixel atual sendo desenhado
    input wire video_active,     // 1 = Está na área visível da tela

    input wire [11:0] ball_x,    // Coordenada X atual da bola
    input wire [11:0] ball_y,    // Coordenada Y atual da bola
    input wire [11:0] paddle1_y, // Coordenada Y do Jogador 1
    input wire [11:0] paddle2_y, // Coordenada Y do Jogador 2

    // Saída de cor para módulo HDMI, 24 bits divididos em 3 canais de 8 bits
    output wire [23:0] rgb_out
);

    // ===== Parâmetros de tamanho =====
    localparam BALL_SIZE = 10;
    localparam PADDLE_W = 15;
    localparam PADDLE_H = 60; 
    localparam PADDLE1_X = 20;  // O paddle esquerdo fica fixo na coluna(x) 20 da tela
    localparam PADDLE2_X = 605; // O paddle direito fica fixo na coluna(x) 605 da tela

    // ===== Verifica quem é quem =====
    // Verifica se a coordenada sendo desenhada agora está dentro do quadrado da bola ou paddle1 ou paddle2
    // 1 = true, 0 = false
    wire is_ball = (pixel_x >= ball_x) && (pixel_x <= ball_x + BALL_SIZE) &&
                   (pixel_y >= ball_y) && (pixel_y <= ball_y + BALL_SIZE);

    wire is_paddle1 = (pixel_x >= PADDLE1_X) && (pixel_x <= PADDLE1_X + PADDLE_W) &&
                      (pixel_y >= paddle1_y) && (pixel_y <= paddle1_y + PADDLE_H);
                      
    wire is_paddle2 = (pixel_x >= PADDLE2_X) && (pixel_x <= PADDLE2_X + PADDLE_W) &&
                      (pixel_y >= paddle2_y) && (pixel_y <= paddle2_y + PADDLE_H);

    // ===== Seletor de cores =====
    wire [23:0] color;
    
assign color = is_ball    ? 24'hFFFFFF : // Bola = Branca
               is_paddle1 ? 24'h00FF00 : // P1 = Verde
               is_paddle2 ? 24'hFF0000 : // P2 = Vermelho
                            24'h000000 ; // Fundo = Preto
                            
    // ===== Ativa máscara de vídeo =====
    // Só passa a cor se o 'video_active' for 1, se não passa preto.
    assign rgb_out = video_active ? color : 24'h000000;

endmodule
