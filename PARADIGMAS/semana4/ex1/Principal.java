import java.util.Scanner;

public class Principal {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Criação da lâmpada Fluorescente: ");
        System.out.println("Digite seu comprimento e seu estado: ");

        int comprimento = scanner.nextInt();
        int estado = scanner.nextInt();

        LampadaFluorescente lampadaFluorescente = new LampadaFluorescente(estado, comprimento);

        System.out.println("Criação da lâmpada LED: ");
        System.out.println("Digite seu estado: ");

        estado = scanner.nextInt();

        LampadaLed lampadaLed = new LampadaLed(estado);

        System.out.println("Escolha a lampada que deseja apertar o botão: ");
        System.out.println("1 - Lâmpada Fluorescente");
        System.out.println("2 - Lâmpada LED");

        int escolha = scanner.nextInt();

        if (escolha == 1)
            lampadaFluorescente.setEstado();
        else
            lampadaLed.setEstado();
        
        lampadaFluorescente.getEstado();
        lampadaLed.getEstado();

        scanner.close();
    }
}