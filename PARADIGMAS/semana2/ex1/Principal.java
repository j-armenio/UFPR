import java.util.Scanner;

public class Principal {
    public static void main(String[] args) 
    {
        Telefone[] telefones = new Telefone[5];
        int ddd, ddi;
        String numero;
        
        Scanner input = new Scanner(System.in);

        for (int i = 0; i < telefones.length; i++) 
        {
            System.out.println("Cadastre o telefone " + (i+1));

            System.out.println("DDI: ");
            ddi = input.nextInt();

            System.out.println("DDD(1, 55 ou 61): ");
            while (true){
                ddd = input.nextInt();
                if (ddd == 1 || ddd == 55 || ddd == 61)
                    break;
                else {
                    System.out.println("DDD inválido.");
                }
            }
            input.nextLine();

            System.out.println("Número: ");
            numero = input.nextLine();

            telefones[i] = new Telefone();
            telefones[i].cadastrar(ddi, ddd, numero);
        }
        
        for (int i = 0; i < telefones.length; i++)
            telefones[i].exibir();

        input.close();

    }
}