import java.util.Scanner;

public class Principal {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        Telefone[] telefones = new Telefone[5];

        int ddi, ddd;
        String numero;

        // Cadastrar os telefones
        for (int i = 0; i < telefones.length; i++) {
            System.out.println("Cadastre seu telefone " + (i + 1) + ":");

            // Validar ddi
            while (true) {
                System.out.println("Digite seu DDI: ");
                ddi = input.nextInt();
                if (ddi == 55 || ddi == 1 || ddi == 61)
                    break;
                else
                    System.out.println("DDI ínvalido. Insira um código válido.");                
            }

            System.out.println("Digite seu DDD: ");
            ddd = input.nextInt();
            input.nextLine();

            System.out.println("Digite seu número: ");
            numero = input.nextLine();

            telefones[i].cadastrar(ddi, ddd, numero);
        }

        System.out.println("Telefones cadastrados: ");
        for (Telefone telefone : telefones) {
            telefone.exibir();
        }
        
        input.close();
    }
}
