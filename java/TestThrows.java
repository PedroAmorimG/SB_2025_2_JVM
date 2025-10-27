// java/TestThrows.java
package java; // Adicionando o package para corresponder à pasta

import java.io.IOException;

// Esta classe vai gerar um atributo "Exceptions" e um "InnerClasses"
public class TestThrows {

    // 1. Isso gera o atributo "Exceptions" no método
    public void metodoComExcecoes() throws IOException, NullPointerException {
        if (System.currentTimeMillis() % 2 == 0) {
            throw new IOException();
        } else {
            throw new NullPointerException();
        }
    }

    // 2. Isso gera o atributo "InnerClasses" na classe
    class MinhaInnerClass {
    }
}