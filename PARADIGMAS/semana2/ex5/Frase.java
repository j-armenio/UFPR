import java.util.Scanner;

public class Frase {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.print("Digite uma frase: ");
        String frase = input.nextLine();

        String[] palavras = frase.trim().split("\\s+");

        int numeroDePalavras = palavras.length;

        System.out.println("Número de palavras na frase: " + numeroDePalavras);

        input.close(); // Fecha o Scanner
    }
}
