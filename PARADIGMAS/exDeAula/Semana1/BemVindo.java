import java.util.Scanner;

public class BemVindo {
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);
        int num;
        System.out.print("Digite um numero: ");

        num = input.nextInt();

        System.out.println("Aqui:" + num);

        input.close();
    }
}