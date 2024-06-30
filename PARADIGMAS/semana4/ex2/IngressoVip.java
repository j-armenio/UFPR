public class IngressoVip extends Ingresso {
    protected double valorAdicionalVip;

    public IngressoVip(double valor, double valorAdicionalVip) {
        super(valor);
        this.valorAdicionalVip = valorAdicionalVip;
    }

    public double getValorAdicionalVip() {
        return this.valorAdicionalVip;
    }

    public void setValorAdicionalVip(double valorAdicionalVip) {
        this.valorAdicionalVip = valorAdicionalVip;
    }

    @Override
    public void imprimeValor() {
        System.out.println("Valor:" + (valor + valorAdicionalVip));
    }
}
