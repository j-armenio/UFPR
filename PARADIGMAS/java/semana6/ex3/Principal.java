import java.util.*;

public class Principal {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        TreeSet<AlunoMonitor> monitores = new TreeSet<AlunoMonitor>();

        monitores.add(new AlunoMonitor("João Pedro", "GRR20221390", "CL1062"));
        monitores.add(new AlunoMonitor("Saibert", "GRR20235330", "CL182"));
        monitores.add(new AlunoMonitor("Gabriel Lisboa", "GRR20221678", "CL182"));

        for (AlunoMonitor monitor : monitores) {
            System.out.println(monitor);
        }
    }
}
