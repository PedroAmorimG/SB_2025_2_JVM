public class TestMatrix {
    public static void main(String[] args) {
        System.out.println("--- Testando Matriz (multianewarray) ---");
        
        // Gera multianewarray (dimensions = 2)
        int[][] matrix = new int[2][3];
        
        matrix[0][0] = 10;
        matrix[1][2] = 50;
        
        System.out.println("Posicao [0][0]:");
        System.out.println(matrix[0][0]); // Esperado: 10
        
        System.out.println("Posicao [1][2]:");
        System.out.println(matrix[1][2]); // Esperado: 50
        
        System.out.println("Posicao [0][1] (padrao):");
        System.out.println(matrix[0][1]); // Esperado: 0
    }
}