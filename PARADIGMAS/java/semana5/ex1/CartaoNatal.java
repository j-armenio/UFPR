public class CartaoNatal extends CartaoWeb {
    
    public CartaoNatal(String destinatario) {
        super(destinatario);
    }

    @Override
    public void retornarMensagem(String remetente) {
        System.out.println("Te desejo um péssimo Natal, " + getDestinatario() + ".\n" +
            "Com todo ódio, \n" + remetente);
    }
}