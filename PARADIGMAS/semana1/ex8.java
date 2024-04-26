import java.util.Scanner;

public class ex8 {
    public static void main(String args[])
    {
        Scanner input = new Scanner(System.in);
        int peso, altura, qtdAcimaDoPeso;
        peso = altura = qtdAcimaDoPeso = 0;

        System.out.println("Digite o peso: ");
        peso = input.nextInt();
        System.out.println("Digite a altura: ");
        altura = input.nextInt();

        while (peso != -1 || altura != -1) {
            if (peso/(altura*altura) <= 25)
                qtdAcimaDoPeso++;

            System.out.println("Digite o peso: ");
            peso = input.nextInt();
            System.out.println("Digite a altura: ");
            altura = input.nextInt();
        }

        System.out.println("Quantidade acima do peso: " + qtdAcimaDoPeso);
        input.close();
    }
}
