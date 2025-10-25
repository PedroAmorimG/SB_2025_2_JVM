public class TestException {
    public static void main(String[] args) {
        try {
            // Força uma divisão por zero
            int x = 10 / 0; 
        } catch (ArithmeticException e) {
            System.out.println("Capturou a excecao!");
        }
    }
}