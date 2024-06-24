import java.util.Scanner;

public class FormataNome {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        String nome = input.nextLine();

        String[] nomes = nome.split("\\s+");

        System.out.println(nomes[nomes.length - 1] + ", " + nomes[0]);
    }
}
