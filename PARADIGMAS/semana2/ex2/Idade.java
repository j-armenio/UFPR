import java.util.Scanner;

public class Idade {
    public static void main(String[] args) 
    {
        int[] idades = new int[10];
        Scanner input = new Scanner(System.in);

        for (int i = 0; i < idades.length; i++) {
            System.out.print("Idade " + (i+1) + ":");
            idades[i] = input.nextInt();
        }

        int media = 0;
        for (int i = 0; i < idades.length; i++)
            media += idades[i];
        media = media/idades.length;

        System.out.println("Acima da média: ");
        for (int i = 0; i < idades.length; i++) {
            if (idades[i] > media)
                System.out.println(idades[i]);
        }

        System.out.println("Abaixo da média: ");
        for (int i = 0; i < idades.length; i++) {
            if (idades[i] <= media)
                System.out.println(idades[i]);
        }

        input.close();
    }
}
