public abstract class Imovel {
    protected String endereco;
    protected double preco;

    public Imovel(String endereco, double preco) {
        setEndereco(endereco);
        setPreco(preco);
    }

    public void setEndereco(String endereco) {
        this.endereco = endereco;
    }
    public void setPreco(double preco) {
        this.preco = preco;
    }
    public String getEndereco() {
        return this.endereco;
    }
    public double getPreco() {
        return this.preco;
    }


    public abstract double calcularValorImovel();
}