import java.util.Scanner;

public class ex7 {
    public static void main(String args[])
    {
        Scanner input = new Scanner(System.in);
        int num, maiorAluno, maiorTurma;
        maiorAluno = maiorTurma = 0;

        for (int i = 1; i <= 30; i++) 
        {
            System.out.println("Aluno " + i);

            for (int j = 1; j <= 4; j++)
            {
                num = input.nextInt();
                if (j == 1)
                    maiorAluno = num;   
                else {
                    if (num > maiorAluno) {
                        maiorAluno = num;
                    }
                }
            }            
            if (i == 1)
                maiorTurma = maiorAluno;
            else {
                if (maiorAluno > maiorTurma)
                    maiorTurma = maiorAluno;
            }
            System.out.println("Maior nota: " + maiorAluno);
        }
        System.out.println("Maior nota da turma: " + maiorTurma);
        input.close();
    }    
}
