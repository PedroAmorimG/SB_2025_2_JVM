
public class PrintArgs {

    public static void main(String[] args) {
        // ---------------------------------
        int[] numeros = new int[5]; // teste de newarray
        numeros[0] = 10;
        // ---------------------------------

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
