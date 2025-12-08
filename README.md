# JVM Interpreter - Manual de Execução

## 🛠️ Pré-requisitos
* **C++**: MinGW 32-bit (ou ambiente que suporte `-m32`).
* **Java**: JDK 8.

## 1. Compilação do Projeto

### Compilar a JVM (C++)
Gera o executável `jvm.exe`. É obrigatório usar 32-bits devido aos ponteiros internos.
```
g++ -std=c++11 -m32 main.cpp classfile/*.cpp runtime/*.cpp -o jvm
```

## 2. Compilar a Biblioteca Nativa (Java)
Compila as classes base (String, System, etc) na pasta runtime.

```
javac --patch-module java.base=runtime runtime/java/lang/*.java runtime/java/io/*.java
```

## 3. Rodando Exemplos
Nota: Sempre compile os .java com --release 8 para garantir compatibilidade.
- Hello World com argumentos
```
javac --release 8 exemplos/PrintArgs.java
./jvm -f exemplos/PrintArgs.class -i -a "Ola Mundo" 123
```
- Teste de tipos e arrays
```
javac --release 8 exemplos/TestFeatures.java
./jvm -f exemplos/TestFeatures.class -i
```
