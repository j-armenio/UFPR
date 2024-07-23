import java.util.Scanner;

public class Nota {
    public static void main(String[] args) {
        int[][] notas = new int[30][5];
        Scanner input = new Scanner(System.in);

        int maiorAluno, maiorTurma;
        maiorTurma = 0;

        for (int i = 0; i < notas.length; i++) {
            System.out.println("Notas do aluno " + (i + 1) + ": ");
            maiorAluno = 0;
            for (int j = 0; j < (notas[i].length - 1); j++) {
                notas[i][j] = input.nextInt();
                if (notas[i][j] > maiorAluno) // Salva maior nota aluno
                    maiorAluno = notas[i][j];
            }
            notas[i][4] = maiorAluno;
            System.out.println(notas[i][4]);
        }

        for (int i = 0; i < notas.length; i++) {
            for (int j = 0; j < (notas[i].length - 1); j++)
                System.out.println("Maior nota aluno " + (i + 1) + ":" + notas[i][4]);
        }
        System.out.println("Maior nota da turma "+  maiorTurma);
    }
}
