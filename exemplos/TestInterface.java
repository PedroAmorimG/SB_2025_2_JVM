// Interface simples
interface Animal {
    void fazerSom();
}

class Cachorro implements Animal {
    public void fazerSom() {
        System.out.println("Au Au!");
    }
}

class Gato implements Animal {
    public void fazerSom() {
        System.out.println("Miau!");
    }
}

public class TestInterface {
    public static void main(String[] args) {
        System.out.println("--- Testando invokeinterface ---");
        
        Animal a1 = new Cachorro();
        Animal a2 = new Gato();
        
        System.out.println("Cachorro diz: ");
        a1.fazerSom();
        
        System.out.println("Gato diz: ");
        a2.fazerSom();
    }
}