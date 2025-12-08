# JVM Interpreter - Software Básico 2025/2

Uma implementação didática de uma Java Virtual Machine (JVM) escrita em C++, desenvolvida para a disciplina de Software Básico. O projeto é capaz de ler arquivos `.class` (Java 8), interpretar bytecodes, gerenciar pilha de execução e memória (Heap/Method Area), e executar programas Java simples.

## Funcionalidades Implementadas

* **Leitura de ClassFile**: Parser completo de arquivos `.class` (Constant Pool, Fields, Methods, Attributes).
* **Instruções (Opcodes)**:
    * Aritmética e Lógica (inteiros, long, float, double).
    * Controle de Fluxo (`if`, `goto`, `tableswitch`, `lookupswitch`).
    * Manipulação de Stack (`pop`, `dup`, `swap`).
    * **Arrays**: Criação e manipulação de arrays primitivos (`newarray`), de referências (`anewarray`) e multidimensionais (`multianewarray`).
    * **Orientação a Objetos**: Instanciação (`new`), acesso a campos (`getfield`/`putfield`), chamadas de métodos (`invokevirtual`, `invokestatic`, `invokespecial`, `invokeinterface`).
    * **Checagem de Tipos**: Suporte a `instanceof` e `checkcast`.
* **Runtime**:
    * Sistema de classes nativas simulado (`java.lang.String`, `java.lang.System`, `java.io.PrintStream`).
    * Suporte a argumentos de linha de comando (`main(String[] args)`).

## Pré-requisitos

* **Compilador C++**: `g++` com suporte a C++11 (Recomendado **MinGW 32-bit** no Windows devido ao gerenciamento de ponteiros).
* **Java JDK**: JDK 8 (para compilar os arquivos `.java`).

## Compilação

### 1. Compilar a JVM (C++)

Como a JVM utiliza ponteiros de 32 bits (`u4`) internamente, é essencial compilar com a flag `-m32`.

```bash
g++ -std=c++11 -m32 main.cpp classfile/*.cpp runtime/*.cpp -o jvm
