// 1. Uma interface genérica simples
interface MeuComparador<T> {
    int comparar(T a, T b);
}

// 2. A classe que implementa a interface com um tipo específico
public class TestBridgeMethod implements MeuComparador<String> {

    // 3. Este é o método que você escreve
    public int comparar(String a, String b) {
        return a.compareTo(b);
    }

    /*
     * 4. O COMPILADOR VAI CRIAR ESTE MÉTODO SINTÉTICO:
     *
     * public synthetic bridge int comparar(Object a, Object b) {
     * return this.comparar((String)a, (String)b);
     * }
     *
     * Este método é sintético e também é uma "ponte" (bridge).
     */

    public static void main(String[] args) {
        TestBridgeMethod t = new TestBridgeMethod();
        System.out.println(t.comparar("A", "B"));
    }
}