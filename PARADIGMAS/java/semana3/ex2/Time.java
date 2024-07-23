public class Time {
    private int hora;
    private int minuto;

    public int getHora() {
        return this.hora;
    }
    public int getMinuto() {
        return this.minuto;
    }
    public void setHora(int hora) {
        this.hora = hora;
    }
    public void setMinuto(int minuto) {
        this.minuto = minuto;
    }

    public boolean anterior(int h, int m) {
        if (h > this.hora) {
            return true;
        } else if (h == this.hora) {
            if (m > this.minuto)
                return true;
        }
        return false;
    }

    public boolean posterior(int h, int m) {
        if (h < this.hora) {
            return true;
        } else if (h == this.hora) {
            if (m < this.minuto)
                return true;
        }
        return false;
    }

    public boolean equals(int h, int m) {
        if (h == this.hora && m == this.minuto)
            return true;
        return false;
    }

    public String toString() {
        return String.format("%d:%d", this.hora, this.minuto);
    }
}
