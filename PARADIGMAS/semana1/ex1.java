import java.util.Scanner;

// Tudo dentro do Java tem que estar dentro de classes
public class ex1 {

    // Método que inicia a execução do programa
    public static void main(String args[]) 
    {
        Scanner input = new Scanner(System.in);
        int num;

        System.out.println("Digite um número: ");
        num = input.nextInt();

        if (num >= 0) {
            System.out.println(num + " é positivo.");
        } else {
            System.out.println(num + " é negativo.");
        }

        input.close();
    }
}