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

    input wire [2:0] p1_score,
    input wire [2:0] p2_score,

    // Saída de cor para módulo HDMI, 24 bits divididos em 3 canais de 8 bits
    output wire [23:0] rgb_out
);

    // ===== Parâmetros =====
    // De tamanho
    localparam BALL_SIZE = 10;
    localparam PADDLE_W = 15;
    localparam PADDLE_H = 60; 
    localparam PADDLE1_X = 20;  // O paddle esquerdo fica fixo na coluna(x) 20 da tela
    localparam PADDLE2_X = 605; // O paddle direito fica fixo na coluna(x) 605 da tela

    // Do placar
    localparam SCORE_Y = 40;
    localparam SCORE1_X = 200;
    localparam SCORE2_X = 400;
    localparam CHAR_W = 8;
    localparam CHAR_H = 16;
    localparam SCALE = 4; // Amplia o texto 4x na tela

    // ===== Verifica quem é quem =====
    // Verifica se a coordenada sendo desenhada agora está dentro do quadrado da bola ou paddle1 ou paddle2
    // 1 = true, 0 = false
    wire is_ball = (pixel_x >= ball_x) && (pixel_x <= ball_x + BALL_SIZE) &&
                   (pixel_y >= ball_y) && (pixel_y <= ball_y + BALL_SIZE);

    wire is_paddle1 = (pixel_x >= PADDLE1_X) && (pixel_x <= PADDLE1_X + PADDLE_W) &&
                      (pixel_y >= paddle1_y) && (pixel_y <= paddle1_y + PADDLE_H);
                      
    wire is_paddle2 = (pixel_x >= PADDLE2_X) && (pixel_x <= PADDLE2_X + PADDLE_W) &&
                      (pixel_y >= paddle2_y) && (pixel_y <= paddle2_y + PADDLE_H);

    // ===== Listra pontilhada no meio =====
    // O centro da tela é 320. A listra vai do 318 ao 321 (4 pixels de largura).
    // (pixel_y[4] == 1'b1) faz a listra existir por 16 pixels, depois sumir por 16 pixels.
    wire is_center_line = (pixel_x >= 318) && (pixel_x <= 321) && (pixel_y[4] == 1'b1);

    // ===== Lógica do Placar =====
    wire is_score1_area = (pixel_x >= SCORE1_X) && (pixel_x < SCORE1_X + (CHAR_W * SCALE)) &&
                          (pixel_y >= SCORE_Y) && (pixel_y < SCORE_Y + (CHAR_H * SCALE));

    wire is_score2_area = (pixel_x >= SCORE2_X) && (pixel_x < SCORE2_X + (CHAR_W * SCALE)) &&
                          (pixel_y >= SCORE_Y) && (pixel_y < SCORE_Y + (CHAR_H * SCALE));

    wire [2:0] current_char = is_score1_area ? p1_score : p2_score;

    wire [2:0] rom_col = is_score1_area ? ((pixel_x - SCORE1_X) / SCALE) :
                         is_score2_area ? ((pixel_x - SCORE2_X) / SCALE) : 0;

    wire [3:0] rom_row = (pixel_y - SCORE_Y) / SCALE;

    wire score_pixel_active;

    // Instancia a ROM
    font_rom u_font (
        .character(current_char),
        .row(rom_row),
        .col(rom_col),
        .pixel_out(score_pixel_active)
    );

    // Combina a área do placar com a resposta da ROM
    wire is_score = (is_score1_area || is_score2_area) && score_pixel_active;

    // ===== Seletor de cores =====
    wire [23:0] color;

    assign color = is_score       ? 24'h333333 : // Placar = Cinza escuro
                   is_ball        ? 24'hFFFFFF : // Bola   = Branco
                   is_paddle1     ? 24'hFFFFFF : // P1     = Branco
                   is_paddle2     ? 24'hFFFFFF : // P2     = Branco
                   is_center_line ? 24'h333333 : // Listra = Cinza Escuro 
                                    24'h000000 ; // Fundo  = Preto

    // Só passa a cor se o 'video_active' for 1, se não passa preto.
    assign rgb_out = video_active ? color : 24'h000000;


endmodule
