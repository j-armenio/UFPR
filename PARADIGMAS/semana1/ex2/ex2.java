import java.util.Scanner;

public class ex2 {
    public static void main(String args[]) 
    {
        Scanner input = new Scanner(System.in);
        int num;
        System.out.println("Digite um número: ");
        num = input.nextInt();

        if (num % 2 == 0)
            System.out.println("É par.");
        else
            System.out.println("É ímpar.");

        input.close();
    }
}
