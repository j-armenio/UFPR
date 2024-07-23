public class LampadaFluorescente extends Lampada {
    private int comprimento;

    public LampadaFluorescente(int estado, int comprimento) {
        super(estado);
        this.comprimento = comprimento;
    }

    public int getComprimento() {
        return comprimento;
    }

    public void setComprimento(int comprimento) {
        this.comprimento = comprimento;
    }
}
