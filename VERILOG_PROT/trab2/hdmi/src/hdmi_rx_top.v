module hdmi_rx_top (
    input        sys_clk, // clock base de 27 MHz
    
    // Entradas físicas do conector HDMI
    input        hdmi_clk_p, hdmi_clk_n,     // par diferencial do clock do HDMI
    input        hdmi_data0_p, hdmi_data0_n, // canal 0 (azul + sincronismo)
    input        hdmi_data1_p, hdmi_data1_n, // canal 1 (verde)
    input        hdmi_data2_p, hdmi_data2_n, // canal 2 (vermelho)
    
    // Sinais que alimentam a tela
    output       video_clk,   // clock de pixel
    output       video_vsync, // sincronismo vertical
    output       video_hsync, // sincronismo horizontal
    output       video_de,    // data enable (alto = pixel válido na tela)
    output [7:0] video_r,     // canal vermelho
    output [7:0] video_g,     // canal verde
    output [7:0] video_b      // canal azul
);

// Sinais internos que carregam sinal elétrico "limpo" pós-conversão
wire clk_in_raw;
wire [2:0] data_in_raw;

// fios de relógio estruturados pelo PLL
wire clk_pixel;
wire clk_serial;
wire pll_lock;

// barramentos de 10 bits contendo as palavras TMDS brutas deserializadas
wire [9:0] tmds_ch0;
wire [9:0] tmds_ch1;
wire [9:0] tmds_ch2;

// sinais de calibração
wire [2:0] tmds_calib = 3'b000;

// o clock de pixel final é derivado do clk_in_war usando o pll
assign video_clk = clk_in_raw;


// ==== Convertendo sinal diferencial físico em lógico interno ====

// buffer para clock do hdmi
TLVDS_IBUF u_clk_ibuf (
    .O(clk_in_raw),
    .I(hdmi_clk_p),
    .IB(hdmi_clk_n)
);

// buffer para o Canal de Dados 0
TLVDS_IBUF u_data0_ibuf (
    .O(data_in_raw[0]),
    .I(hdmi_data0_p),
    .IB(hdmi_data0_n)
);

// buffer para o Canal de Dados 1
TLVDS_IBUF u_data1_ibuf (
    .O(data_in_raw[1]),
    .I(hdmi_data1_p),
    .IB(hdmi_data1_n)
);

// buffer para o Canal de Dados 2
TLVDS_IBUF u_data2_ibuf (
    .O(data_in_raw[2]),
    .I(hdmi_data2_p),
    .IB(hdmi_data2_n)
);

// ===== Gerador de clock (multiplicação de frequencia para amostragem) =====
hdmi_clock_gen u_clock_gen (
    .clk_hdmi_in(clk_in_raw),
    .clk_pixel(clk_pixel),
    .clk_serial(clk_serial),
    .pll_lock(pll_lock)
);

// ==== Deserializados (1 bit serial -> 10 bits paralelos)
hdmi_deserializer u_deserializer (
        .clk_pixel(clk_pixel),
        .clk_serial(clk_serial),
        .reset(!pll_lock), // Mantém em reset até o clock do PLL estabilizar
        .data_in_raw(data_in_raw),
        .calib(tmds_calib),
        .data_tmds0(tmds_ch0),
        .data_tmds1(tmds_ch1),
        .data_tmds2(tmds_ch2)
    );

endmodule