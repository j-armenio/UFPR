import java.util.Scanner;

public class Principal {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        final String remetente = "João";

        CartaoWeb[] cartoes = new CartaoWeb[3];
        System.out.println("Insira o destinatário do cartão de dia dos namorados: ");
        cartoes[0] = new CartaoDiaDosNamorados(scanner.nextLine());
        System.out.println("Insira o destinatário do cartão de natal: ");
        cartoes[1] = new CartaoNatal(scanner.nextLine());
        System.out.println("Insira o destinatário do cartão de aniversário: ");
        cartoes[2] = new CartaoAniversario(scanner.nextLine());

        for (int i=0; i<3; i++) {
            cartoes[i].retornarMensagem(remetente);
            System.out.println();
        }

        scanner.close();
    }
}
