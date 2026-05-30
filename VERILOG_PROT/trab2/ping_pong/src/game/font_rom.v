module font_rom (
    input wire [2:0] character, // Qual número desenhar (0 a 5)
    input wire [3:0] row,       // Linha atual do caractere (0 a 15)
    input wire [2:0] col,       // Coluna atual do caractere (0 a 7)
    output wire pixel_out       // O pixel nesta exata posição (1=branco, 0=vazio)
);

    wire [7:0] num_rom [0:5][0:15];

    // Número 0
    assign num_rom[0][0]  = 8'b00000000; assign num_rom[0][1]  = 8'b00111100;
    assign num_rom[0][2]  = 8'b01100110; assign num_rom[0][3]  = 8'b01100110;
    assign num_rom[0][4]  = 8'b01100110; assign num_rom[0][5]  = 8'b01100110;
    assign num_rom[0][6]  = 8'b01100110; assign num_rom[0][7]  = 8'b01100110;
    assign num_rom[0][8]  = 8'b01100110; assign num_rom[0][9]  = 8'b01100110;
    assign num_rom[0][10] = 8'b01100110; assign num_rom[0][11] = 8'b01100110;
    assign num_rom[0][12] = 8'b01100110; assign num_rom[0][13] = 8'b01100110;
    assign num_rom[0][14] = 8'b00111100; assign num_rom[0][15] = 8'b00000000;

    // Número 1
    assign num_rom[1][0]  = 8'b00000000; assign num_rom[1][1]  = 8'b00011000;
    assign num_rom[1][2]  = 8'b00111000; assign num_rom[1][3]  = 8'b01111000;
    assign num_rom[1][4]  = 8'b00011000; assign num_rom[1][5]  = 8'b00011000;
    assign num_rom[1][6]  = 8'b00011000; assign num_rom[1][7]  = 8'b00011000;
    assign num_rom[1][8]  = 8'b00011000; assign num_rom[1][9]  = 8'b00011000;
    assign num_rom[1][10] = 8'b00011000; assign num_rom[1][11] = 8'b00011000;
    assign num_rom[1][12] = 8'b00011000; assign num_rom[1][13] = 8'b00011000;
    assign num_rom[1][14] = 8'b01111110; assign num_rom[1][15] = 8'b00000000;

    // Número 2
    assign num_rom[2][0]  = 8'b00000000; assign num_rom[2][1]  = 8'b00111100;
    assign num_rom[2][2]  = 8'b01100110; assign num_rom[2][3]  = 8'b01100110;
    assign num_rom[2][4]  = 8'b00000110; assign num_rom[2][5]  = 8'b00001100;
    assign num_rom[2][6]  = 8'b00011000; assign num_rom[2][7]  = 8'b00110000;
    assign num_rom[2][8]  = 8'b01100000; assign num_rom[2][9]  = 8'b01100000;
    assign num_rom[2][10] = 8'b01100000; assign num_rom[2][11] = 8'b01100000;
    assign num_rom[2][12] = 8'b01100000; assign num_rom[2][13] = 8'b01100000;
    assign num_rom[2][14] = 8'b01111110; assign num_rom[2][15] = 8'b00000000;

    // Número 3
    assign num_rom[3][0]  = 8'b00000000; assign num_rom[3][1]  = 8'b00111100;
    assign num_rom[3][2]  = 8'b01100110; assign num_rom[3][3]  = 8'b01100110;
    assign num_rom[3][4]  = 8'b00000110; assign num_rom[3][5]  = 8'b00000110;
    assign num_rom[3][6]  = 8'b00001100; assign num_rom[3][7]  = 8'b00111000;
    assign num_rom[3][8]  = 8'b00001100; assign num_rom[3][9]  = 8'b00000110;
    assign num_rom[3][10] = 8'b00000110; assign num_rom[3][11] = 8'b00000110;
    assign num_rom[3][12] = 8'b01100110; assign num_rom[3][13] = 8'b01100110;
    assign num_rom[3][14] = 8'b00111100; assign num_rom[3][15] = 8'b00000000;

    // Número 4
    assign num_rom[4][0]  = 8'b00000000; assign num_rom[4][1]  = 8'b00000110;
    assign num_rom[4][2]  = 8'b00001110; assign num_rom[4][3]  = 8'b00011110;
    assign num_rom[4][4]  = 8'b00100110; assign num_rom[4][5]  = 8'b01100110;
    assign num_rom[4][6]  = 8'b01100110; assign num_rom[4][7]  = 8'b01100110;
    assign num_rom[4][8]  = 8'b01111111; assign num_rom[4][9]  = 8'b00000110;
    assign num_rom[4][10] = 8'b00000110; assign num_rom[4][11] = 8'b00000110;
    assign num_rom[4][12] = 8'b00000110; assign num_rom[4][13] = 8'b00000110;
    assign num_rom[4][14] = 8'b00000110; assign num_rom[4][15] = 8'b00000000;

    // Número 5
    assign num_rom[5][0]  = 8'b00000000; assign num_rom[5][1]  = 8'b01111110;
    assign num_rom[5][2]  = 8'b01100000; assign num_rom[5][3]  = 8'b01100000;
    assign num_rom[5][4]  = 8'b01100000; assign num_rom[5][5]  = 8'b01100000;
    assign num_rom[5][6]  = 8'b01111100; assign num_rom[5][7]  = 8'b01100110;
    assign num_rom[5][8]  = 8'b00000110; assign num_rom[5][9]  = 8'b00000110;
    assign num_rom[5][10] = 8'b00000110; assign num_rom[5][11] = 8'b00000110;
    assign num_rom[5][12] = 8'b01100110; assign num_rom[5][13] = 8'b01100110;
    assign num_rom[5][14] = 8'b00111100; assign num_rom[5][15] = 8'b00000000;

    // Saída via assign (Combinacional). Se for um caractere válido, busca na matriz.
    assign pixel_out = (character <= 5) ? num_rom[character][row][~col] : 1'b0;

endmodule
