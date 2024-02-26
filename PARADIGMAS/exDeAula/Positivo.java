import java.util.Scanner;

public class Positivo {
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);
        int num;

        num = input.nextInt();

        if (num >= 0) {
            System.out.println(num + " é positivo.");
        } else {
            System.out.println(num + " é negativo.");
        }
    }
}