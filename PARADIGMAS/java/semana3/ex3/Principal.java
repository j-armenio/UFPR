import java.util.Scanner;

public class Principal {
    public static void main(String[] args) 
    {
        Scanner input = new Scanner(System.in);
        Data data1 = new Data();
        Data data2 = new Data();

        /* 
        * !!! Validação Falha, caso a data seja invalida não pede para re-inserir !!!
        * A função set faz a validação, porém não retorna valor nenhum, não sei como 
        * fazer para fazer essa verificação, tem que fazer pela main? colocar o set para retornar?
        */ 

        System.out.println("Insira a primeira data: ");
        System.out.print("Dia: ");
        data1.setDia(input.nextInt());
        System.out.print("Mes: ");
        data1.setMes(input.nextInt());
        System.out.print("Ano: ");
        data1.setAno(input.nextInt());

        System.out.println("Insira a segunda data: ");
        System.out.print("Dia: ");
        data2.setDia(input.nextInt());
        System.out.print("Mes: ");
        data2.setMes(input.nextInt());
        System.out.print("Ano: ");
        data2.setAno(input.nextInt());

        System.out.println("Data 1: " + data1.toString());
        System.out.println("Data 2: " + data2.toString());

        if ( data1.equals(data2.getDia(), data2.getMes(), data2.getAno()) )
            System.out.println("Datas iguais.");
        else if ( data1.anterior(data2.getDia(), data2.getMes(), data2.getAno()) )
            System.out.println("Data 1 é anterior.");
        else if ( data1.posterior(data2.getDia(), data2.getMes(), data2.getAno()) )
            System.out.println("Data 1 é posterior.");

        input.close();
    }
}
