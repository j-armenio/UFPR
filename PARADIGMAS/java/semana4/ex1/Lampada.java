public class Lampada {
    protected int estado;
    
    public Lampada(int estado) {
        if (estado == 1 || estado == 0)
            this.estado = estado;
        else
            System.out.println("Estado inválido");
    }   

    public void getEstado() {
        if (estado == 1)
            System.out.println("Ligada");
        else
            System.out.println("Desligada");
    }

    public void setEstado() {
        if (estado == 1)
            estado = 0;
        else
            estado = 1;
    }


}