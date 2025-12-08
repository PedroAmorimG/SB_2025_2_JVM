public class TestFeatures {

    public static void main(String[] args) {
        // Testes anteriores
        testInstanceof();
        // testBastore(); // Pode comentar se quiser focar no checkcast
        
        testAllArrayTypes();

        // Novo teste
        testCheckcast();

    }

    public static void testAllArrayTypes() {
        System.out.println("--- Testando Todos Arrays (xastore) ---");
        
        // Long (lastore + ldc2_w)
        long[] longs = new long[1];
        longs[0] = 1234567890123L; 
        System.out.println("[OK] Long: ");
        System.out.println(longs[0]);

        // Float (fastore + ldc)
        float[] floats = new float[1];
        floats[0] = 3.14f; 
        System.out.println("[OK] Float: ");
        System.out.println(floats[0]);

        // Double (dastore + ldc2_w)
        double[] doubles = new double[1];
        doubles[0] = 99.999; 
        System.out.println("[OK] Double: "); 
        System.out.println(doubles[0]);      

        // Char (castore + bipush)
        char[] chars = new char[2];
        chars[0] = 'O'; 
        chars[1] = 'K'; 
        
        System.out.println("[OK] Char: ");
        System.out.println(chars[0]);
        System.out.println(chars[1]);
    }

    public static void testInstanceof() {
        System.out.println("--- Testando instanceof ---");
        String texto = "Ola";
        if (texto instanceof String) {
            System.out.println("[OK] String eh instancia de String");
        }
        if (texto instanceof Object) {
            System.out.println("[OK] String eh instancia de Object");
        }
        Object obj = new Object();
        if (obj instanceof String) {
            System.out.println("[ERRO] Object nao deveria ser String");
        } else {
            System.out.println("[OK] Object nao eh String");
        }
    }

    public static void testCheckcast() {
        System.out.println("\n--- Testando checkcast ---");

        Object objStr = "Sou uma String";
        Object objGen = new Object();

        // CASO 1: Cast Válido (Deve passar)
        // O compilador gera: checkcast #index (java/lang/String)
        System.out.println("Tentando cast valido (Object -> String)...");
        String s = (String) objStr;
        // ISTO USA APENAS O QUE VOCÊ JÁ TEM:
        System.out.println("[OK] Cast valido funcionou! Valor a seguir:");
        System.out.println(s);

        // CASO 2: Cast de Null (Deve passar sempre)
        System.out.println("Tentando cast de null...");
        String nulo = (String) null;
        System.out.println("[OK] Cast de null funcionou.");

        // CASO 3: Cast Inválido (DEVE FALHAR)
        // Aqui estamos forçando um Object genérico a virar String.
        // Sua JVM deve lançar a exceção e parar aqui.
        System.out.println("Tentando cast invalido (Object -> String)...");
        System.out.println("Vai travar AGORA com erro fatal:");
        
        String falha = (String) objGen; // <--- O checkcast vai pegar isso aqui!
        
        System.out.println("[ERRO] O programa nao deveria chegar aqui!");
    }
}