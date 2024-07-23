public class CartaoAniversario extends CartaoWeb {

    public CartaoAniversario(String destinatario) {
        super(destinatario);
    }

    @Override
    public void retornarMensagem(String remetente) {
        System.out.println("Feliz aníversario, " + getDestinatario() + ", " +
                            "espero que morra logo." +
                            "Com amor, " + remetente);
    }
}