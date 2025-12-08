#pragma once

#include "array.h"
#include "runtime_class_types.h"
#include <string>
#include <unordered_map>

/**
 * @typedef NativeMethod
 * @brief Ponteiro de função para implementações de métodos nativos.
 * Recebe o Frame atual para acessar argumentos na pilha de operandos.
 */
using NativeMethod = void (*)(Frame &frame);
/**
 * @brief Carrega e registra os métodos nativos suportados pela JVM.
 * Preenche o mapa com assinaturas (ex: "java/io/PrintStream.println(I)V") e suas funções C++.
 * @param native_methods Ponteiro para o mapa onde os métodos serão armazenados.
 */
void load_map(std::unordered_map<std::string, NativeMethod> *native_methods);

// Implementações de Métodos Nativos (System.out)
void print_ln(Frame &frame);
/** @brief Implementa System.out.println() (nova linha vazia). */
void print_str_ln(Frame &frame);
/** @brief Implementa System.out.println(int). */
void print_int_ln(Frame &frame);
/** @brief Implementa System.out.println(long). */
void print_lg_ln(Frame &frame);
/** @brief Implementa System.out.println(boolean). */
void print_bl_ln(Frame &frame);
/** @brief Implementa System.out.println(float). */
void print_fl_ln(Frame &frame);
/** @brief Implementa System.out.println(double). */
void print_db_ln(Frame &frame);
/** @brief Implementa System.out.println(char). */
void print_char_ln(Frame &frame);

// Implementações de Métodos Nativos (String)

/** @brief Implementa String.length(). */
void str_len(Frame &frame);
/** @brief Implementa String.charAt(int). */
void str_char_at(Frame &frame);
/** @brief Implementa String.equals(Object). */
void str_eq(Frame &frame);
/** @brief Implementa String.getBytes(). Retorna array de bytes. */
void str_get_bytes(Frame &frame);
