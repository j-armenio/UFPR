public class AlunoMonitor implements Comparable<AlunoMonitor> {
    private String nome;
    private String matricula;
    private String codigo_disciplina;

    public AlunoMonitor(String nome, String matricula, String codigo_disciplina) {
        setNome(nome);
        setMatricula(matricula);
        setCodigoDisciplina(codigo_disciplina);
    }

    public void setNome(String nome) {
        this.nome = nome;
    };
    public void setMatricula(String matricula) {
        this.matricula = matricula;
    };
    public void setCodigoDisciplina(String codigo_disciplina) {
        this.codigo_disciplina = codigo_disciplina;
    };
    public String getNome() {
        return this.nome;
    };
    public String getMatricula() {
        return this.matricula;
    };
    public String getCodigoDisciplina() {
        return this.codigo_disciplina;
    };

    @Override
    public int compareTo(AlunoMonitor a) {
        return this.nome.compareTo(a.nome);
    }

    @Override
    public String toString() {
        return "Nome: " + nome + 
                ", Matrícula: " + matricula + 
                ", Código da disciplina : " +  codigo_disciplina;
    }
}
