module hdmi_deserializer (
    input        clk_pixel,   // Clock de pixel mais lento (1x)
    input        clk_serial,  // Clock serial ultra-rápido (5x)
    input        reset,       // Reset do sistema
    input [2:0]  data_in_raw, // Os 3 fios de dados que vieram dos buffers LVDS
    input [2:0]  calib,       // Sinais de calibração para alinhar os bits de cada canal
    output [9:0] data_tmds0,  // Canal 0 convertido para 10 bits paralelos
    output [9:0] data_tmds1,  // Canal 1 convertido para 10 bits paralelos
    output [9:0] data_tmds2   // Canal 2 convertido para 10 bits paralelos
);

    // Desserializador do Canal 0 (Azul + Sincronismos)
    IDES10 u_ides10_ch0 (
        .D(data_in_raw[0]),
        .FCLK(clk_serial),
        .PCLK(clk_pixel),
        .RESET(reset),
        .CALIB(calib[0]),
        .Q(data_tmds0)
    );

    // Desserializador do Canal 1 (Verde)
    IDES10 u_ides10_ch1 (
        .D(data_in_raw[1]),
        .FCLK(clk_serial),
        .PCLK(clk_pixel),
        .RESET(reset),
        .CALIB(calib[1]),
        .Q(data_tmds1)
    );

    // Desserializador do Canal 2 (Vermelho)
    IDES10 u_ides10_ch2 (
        .D(data_in_raw[2]),
        .FCLK(clk_serial),
        .PCLK(clk_pixel),
        .RESET(reset),
        .CALIB(calib[2]),
        .Q(data_tmds2)
    );

endmodule