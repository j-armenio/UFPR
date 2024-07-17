import java.util.*;

public class Principal {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        LinkedList<Professor> professores = new LinkedList<Professor>();

        professores.add(new Professor("Menotti", "123"));
        professores.add(new Professor("Luis", "456"));
        professores.add(new Professor("Todt", "789"));
        professores.add(new Professor("Guilherme", "101"));
        professores.add(new Professor("Paulo", "121"));
        professores.add(new Professor("Matheus", "131"));

        System.out.println("Escolha a matricula do professor a ser removido: ");
        String matriculaParaRemover = input.nextLine();

        Iterator<Professor> iterator = professores.iterator();
        while (iterator.hasNext()) {
            Professor professor = iterator.next();
            if (professor.getMatricula().equals(matriculaParaRemover)){
                iterator.remove();
            }
        }

        for (Professor professor : professores) {
            System.out.println(professor);
        }
    }
}
