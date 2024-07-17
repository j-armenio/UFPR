public class Professor {
    private String nome;
    private String matricula;

    public Professor(String nome, String matricula) {
        setNome(nome);
        setMatricula(matricula);
    }

    public void setNome(String nome) {
        this.nome = nome;
    }
    public void setMatricula(String matricula) {
        this.matricula = matricula;
    }
    public String getNome() {
        return this.nome;
    }
    public String getMatricula() {
        return this.matricula;
    }

    @Override
    public String toString() {
        return "Nome: " + getNome() + 
                ", Matricula: "  + getMatricula();
    }

}
