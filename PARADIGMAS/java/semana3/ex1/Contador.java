/*
 * Liste  o  nome  de  cinco  classes  que  poderiam  ter  como  atributo  um  contador  do tipo desta classe.
 * 1. QuantidadeDeClientes 
 * 2. ItemsEmArmazem
 * 3. CarrinhoDeCompras
 * 4. CarrosEmEstoque
 * 5. PessoasNaFila
 */

public class Contador {
    private int contador;

    public int getContador() {
        return this.contador;
    }

    public void setContador(int contador) {
        this.contador = contador;
    }

    public void zerarContador() {
        this.contador = 0;
    }

    public void incrementaContador() {
        this.contador++;
    }

    public void decrementarContador() {
        this.contador--;
    }
}