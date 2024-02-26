import java.util.Scanner;

public class ParOuImpar {
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);
        int num;
        System.out.print("Digite um numero: ");

        num = input.nextInt();

        if (num % 2 == 0) {
            System.out.print("Número é par");
        } else {
            System.out.print("Número é ímpar");
        }
    }
}