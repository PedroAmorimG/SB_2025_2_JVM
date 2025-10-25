public class TestSynthetic {

    private int campoDaMae = 123;

    // Esta é uma classe interna não-estática
    public class InnerClass {
        
        // Este método acessa um campo privado da classe "mãe"
        public int getCampoDaMae() {
            // Para fazer isso, o compilador precisa de uma
            // referência secreta para a instância de TestSynthetic
            return campoDaMae; 
        }
    }

    // Método apenas para usar a classe interna
    public void usarInnerClass() {
        InnerClass inner = new InnerClass();
        System.out.println(inner.getCampoDaMae());
    }

    public static void main(String[] args) {
        new TestSynthetic().usarInnerClass();
    }
}