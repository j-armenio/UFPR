module hdmi_clock_gen (
    input  clk_hdmi_in,   // Clock bruto de pixel que vem do cabo (74.25 MHz)
    output clk_pixel,     // Clock de pixel limpo para a lógica interna (74.25 MHz)
    output clk_serial,    // Clock de alta velocidade para os desserializadores (371.25 MHz)
    output pll_lock       // Sinal que fica em '1' quando o clock estabiliza
);

    wire clk_fb;
    wire clkout_vco;

    // Primitiva de hardware rPLL da Gowin
    // Configurada para uma multiplicação padrão de 5x
    rPLL #(
        .FBDIV_SEL(5),     // Multiplica o clock por 5 (Feedback Divider)
        .IDDIV_SEL(1),     // Divisor de entrada (1 = mantém)
        .ODDIV_SEL(8),     // Divisor de saída do VCO
        .CLKOUT_DIV(1),    // Divisão do clock de saída principal (Serial)
        .CLKOUTD_DIV(5),   // Divisão do segundo clock (Pixel = Serial / 5)
        .CLKOUT_BYPASS("FALSE"),
        .CLKOUTD_BYPASS("FALSE"),
        .DEVICE("GW1NR-9C")
    ) u_rpll (
        .CLKOUT(clk_serial),    // Saída de alta velocidade (5x)
        .CLKOUTD(clk_pixel),    // Saída de velocidade do pixel (1x)
        .CLKOUTP(),
        .CLKOUTD3(),
        .RESET(1'b0),
        .RESET_P(1'b0),
        .CLKIN(clk_hdmi_in),    // Clock vindo do cabo HDMI
        .CLKFB(clk_fb),
        .FBDSEL(6'b0),
        .IDSEL(6'b0),
        .ODSEL(6'b0),
        .PSDA(4'b0),
        .DUTYDA(4'b0),
        .FDLY(4'b0),
        .LOCK(pll_lock)         // 1 = Clock está estável e confiável
    );

    // O rPLL precisa de um loop de feedback interno para alinhar a fase
    assign clk_fb = clk_pixel; 

endmodule