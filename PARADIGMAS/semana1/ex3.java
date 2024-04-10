import java.util.Scanner;

public class ex3 {
    public static void main(String args[]) 
    {
        Scanner input = new Scanner(System.in);
        int num, soma;
        soma = 0;

        System.out.println("Digite um número: ");
        num = input.nextInt();

        if (num != -1) {
            while (num != -1) {
                soma += num;
                num = input.nextInt();
            }
        }
        System.out.println("soma: " + soma);        

        input.close();
    }
}
