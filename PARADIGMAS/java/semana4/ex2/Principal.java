import java.util.Scanner;

public class Principal {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        double valor, valorAdicionalVip, valorAdicionalCamarote;

        IngressoNormal[] ingressoNormalVet = new IngressoNormal[25];
        IngressoVip[] ingressoVipVet = new IngressoVip[25];
        IngressoCamarote[] ingressoCamaroteVet = new IngressoCamarote[25];
        
        for (int i = 0; i < 25; i++) {
            System.out.println("Insira os valores de seus ingressos: ");

            System.out.println("1. Ingresso Normal: ");
            System.out.println("Insira seu valor: ");
            valor = scanner.nextDouble();
            IngressoNormal ingressoNormal = new IngressoNormal(valor);
            ingressoNormalVet[i] = ingressoNormal;

            System.out.println("2. Ingresso Vip: ");
            System.out.println("Insira seu valor: ");
            valor = scanner.nextDouble();
            System.out.println("Insira seu valor adicional Vip: ");
            valorAdicionalVip = scanner.nextDouble();
            IngressoVip ingressoVip = new IngressoVip(valor, valorAdicionalVip);
            ingressoVipVet[i] = ingressoVip;

            System.out.println("3. Ingresso Camarote: ");
            System.out.println("Insira seu valor: ");
            valor = scanner.nextDouble();
            System.out.println("Insira seu valor adicional Vip: ");
            valorAdicionalVip = scanner.nextDouble();
            System.out.println("Insira seu valor adicional Camarote: ");
            valorAdicionalCamarote = scanner.nextDouble();
            IngressoCamarote ingressoCamarote = new IngressoCamarote(valor, valorAdicionalVip, valorAdicionalCamarote);
            ingressoCamaroteVet[i] = ingressoCamarote;
        }
        
        for (int i = 0; i < 25; i++) {
            ingressoNormalVet[i].imprimeValor();
            ingressoVipVet[i].imprimeValor();
            ingressoCamaroteVet[i].imprimeValor();
        }
        
        scanner.close();
    }    
}
