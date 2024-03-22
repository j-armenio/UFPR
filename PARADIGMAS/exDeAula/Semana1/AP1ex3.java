import java.util.Scanner;

public class AP1ex3 {
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);
        int num;
        System.out.print("Digite um numero: ");

        num = input.nextInt();
        int sum = 0;

        while (num != -1) {
            System.out.print("Digite um numero: ");
            num = input.nextInt();
            
            sum = sum + num;
        }
        
        
        System.out.println(sum);
    }
}