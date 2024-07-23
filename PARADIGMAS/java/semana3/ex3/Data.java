public class Data {
    private int dia;
    private int mes;
    private int ano;

    public int getDia(){
        return this.dia;
    }

    public int getMes(){
        return this.dia;
    }

    public int getAno(){
        return this.dia;
    }

    public void setDia(int dia){
        if (dia <= 31)
            this.dia = dia;
        else
            System.out.println("Dia inválido.");
    }

    public void setMes(int mes){
        if (mes <= 12)
            this.mes = mes;
        else
            System.out.println("Mês inválido.");
    }

    public void setAno(int ano){
        this.ano = ano;
    }

    public boolean anterior(int d, int m, int a) {
        if (this.ano < a)
            return true;
        else if (this.ano == a) {
            if (this.mes < m)
                return true;
        }
        else if (this.ano == a) {
            if (this.mes == m) {
                if (this.dia < d)
                    return true;
            }
        }
        return false;
    }

    public boolean posterior(int d, int m, int a) {
        if (this.ano > a)
            return true;
        else if (this.ano == a) {
            if (this.mes > m)
                return true;
        }
        else if (this.ano == a) {
            if (this.mes == m) {
                if (this.dia > d)
                    return true;
            }
        }
        return false;
    }

    public boolean equals(int d, int m, int a) {
        if (this.ano == a && this.mes == m && this.dia == d)
            return true;
        return false;
    }

    public String toString() {
        return String.format("%d/%d/%d", this.dia, this.mes, this.ano);
    }
}