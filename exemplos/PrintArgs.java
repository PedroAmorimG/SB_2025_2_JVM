
public class PrintArgs {

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Nenhum argumento foi passado.");
            return;
        }

        System.out.println("Argumentos recebidos:");
        for (String arg : args) {
            System.out.println(arg);
        }
    }
}
