import java.util.Scanner;

public class ex5 {
    public static void main(String args[])
    {
        Scanner input = new Scanner(System.in);
        int num;

        System.out.println("Digite um número: ");
        num = input.nextInt();
        
        if (num >= 0 && num < 5)
            {}
        else if (num >= 5 && num < 10)
            num = 2*num + 1;
        else if (num >= 10)
            num = num - 3;            
        
        System.out.println(num);

        input.close();
    }
}
