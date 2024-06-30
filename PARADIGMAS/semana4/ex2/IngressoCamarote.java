public class IngressoCamarote extends IngressoVip { 
    private double valorAdicionalCamarote;

    public IngressoCamarote(double valor, double valorAdicionalVip, double valorAdicionalCamarote) {
        super(valor, valorAdicionalVip);
        this.valorAdicionalCamarote = valorAdicionalCamarote;
    }

    @Override
    public void imprimeValor() {
        System.out.println("Valor: " + (valor + valorAdicionalVip + valorAdicionalCamarote));
    }
}
