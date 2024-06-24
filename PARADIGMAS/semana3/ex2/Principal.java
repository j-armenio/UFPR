import java.util.Scanner;

public class Principal {
    public static void main(String[] args) 
    {
        Scanner input = new Scanner(System.in);
        Time hora1 = new Time();
        Time hora2 = new Time();


        System.out.println("Cadastrando primeiro horário: ");
        System.out.print("Insira a hora: ");
        hora1.setHora(input.nextInt());
        System.out.print("Insira o minuto: ");
        hora1.setMinuto(input.nextInt());

        System.out.println("Cadastrando segundo horário: ");
        System.out.print("Insira a hora: ");
        hora2.setHora(input.nextInt());
        System.out.print("Insira o minuto: ");
        hora2.setMinuto(input.nextInt());

        System.out.println("Horário 1: " + hora1.toString());
        System.out.println("Horário 2: " + hora2.toString());

        if ( hora1.equals(hora2.getHora(), hora2.getMinuto()) )
            System.out.println("Horários iguais.");
        else if ( hora1.anterior(hora2.getHora(), hora2.getMinuto()) )
            System.out.println("Horário 1 é anterior.");
        else if ( hora1.posterior(hora2.getHora(), hora2.getMinuto()) )
            System.out.println("Horário 1 é posterior.");

        input.close();
    }
}
