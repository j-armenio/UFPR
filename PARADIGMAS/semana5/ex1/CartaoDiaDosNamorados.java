public class CartaoDiaDosNamorados extends CartaoWeb {

    public CartaoDiaDosNamorados(String destinatario) {
        super(destinatario);
    }

    @Override
    public void retornarMensagem(String remetente) {
        System.out.println("Querida " + getDestinatario() + "," +
                            "Feliz Dia dos Namorados! \n" +
                            "Espero que esse tenha sido o único cartão do dia dos namorados que tenha \n" +
                            "ganhado nessa data!\n" +
                            "De todo meu coração,\n" +
                            remetente
        );
    }
}