import java.util.Scanner;

public class Matrix {
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);

        double[][] m1 = new double[3][3];
        double[][] m2 = new double[3][3];

        System.out.println("Preencha a matriz 1: ");
        for (int i = 0; i < m1.length; i++) {
            for (int j = 0; j < m1[i].length; j++) {
                m1[i][j] = input.nextDouble();
            }
        }

        System.out.println("Preencha a matriz 2: ");
        for (int i = 0; i < m1.length; i++) {
            for (int j = 0; j < m1[i].length; j++) {
                m2[i][j] = input.nextDouble();
            }
        }

        System.out.println("Soma das matrizes: ");
        for (int i = 0; i < m1.length; i++) {
            for (int j = 0; j < m1[i].length; j++) {
                System.out.print((m1[i][j] + m2[i][j]) + " ");
            }
            System.out.println();
        }
    }
}
