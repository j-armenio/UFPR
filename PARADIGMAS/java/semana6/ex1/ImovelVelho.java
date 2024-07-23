public class ImovelVelho extends Imovel {
    public ImovelVelho(String endereco, double preco) {
        super(endereco, preco);
    }

    public double calcularValorImovel() {
        return getPreco() * 0.7;
    }
}
