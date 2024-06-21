public class Telefone {
    private int ddi;
    private int ddd;
    private String numero;

    public void cadastrar(int ddi, int ddd, String numero){
        this.ddi = ddi;
        this.ddd = ddd;
        this.numero = numero;
    }

    public void exibir() {
        System.out.println("+" + ddi + "(" + ddd + ")" + numero);
    }
}