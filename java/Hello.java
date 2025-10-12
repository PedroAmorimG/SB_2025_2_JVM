package java;

// Classe que implementa a interface
/*
public class Hello {

    // Interface interna
    interface HelloWorld {
        static int a = 5;
        void sayHello(); // método abstrato (contrato)
    }

    // Classe interna que implementa a interface
    static class HelloImpl implements HelloWorld {
        public void sayHello() {
            System.out.println("Olá, mundo!");
            System.out.println(a);
        }
    }

    // Método principal
    public static void main(String[] args) {
        HelloWorld h = new HelloImpl(); // usa a interface como tipo
        h.sayHello();                   // chama o método implementado
    }
}

*/
public class Hello implements HelloWorld {

    // Implementação do método da interface
    public void sayHello() {
        System.out.println("Olá, mundo!");
    }

    // Método principal
    public static void main(String[] args) {
        Hello h = new Hello(); // cria um objeto da classe
        h.sayHello(); // chama o método implementado
    }
}

/*
 * public class Hello {
 * public static void main(String[] args) {
 * System.out.println("Olá, mundo!");
 * }
 * }
 */