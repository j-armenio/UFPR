import java.util.Scanner;

public class Principal {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        Imovel[] imoveis = new Imovel[199];

        for (int i = 0; i < 5; i++) {
            System.out.println("Insira o endereço e preço de seu imóvel: ");
            String endereco = input.nextLine();
            double preco = input.nextDouble();
            input.nextLine();

            if (i % 2 == 0) { // Par
                imoveis[i] = new ImovelNovo(endereco, preco);
            } else { // Ímpar
                imoveis[i] = new ImovelVelho(endereco, preco);
            }
        }

        for (int i = 0; i < 5; i++) {
            System.out.println("Imovel " + i + ", localizado em " + imoveis[i].getEndereco() + ":");
            System.out.println(imoveis[i].calcularValorImovel());
        }

        input.close();
    }    
}
