public class ImovelNovo extends Imovel {
    public ImovelNovo(String endereco, double preco) {
        super(endereco, preco);
    }

    public double calcularValorImovel() {
        return getPreco() * 1.5;
    }
}
