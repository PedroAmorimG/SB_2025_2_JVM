#pragma once

#include "./runtime_class_types.h"
#include <cstddef>
#include <stdexcept>
#include <vector>
/**
 * @class RuntimeArray
 * @brief Representa um array alocado na Heap da JVM.
 *
 * Esta classe lida com dois tipos de armazenamento:
 * 1. Primitivos (int, float, byte, etc.): Armazenados como bytes brutos no vetor `raw`.
 * 2. Referências (Object[], String[], etc.): Armazenados como ponteiros no vetor `refs`.
 */
class RuntimeArray {
public:
  RuntimeClass *component_class;
  bool is_reference;
  size_t length;

  // Para arrays de primitivos, os bytes ficam em raw com passo elem_size.
  /** * @brief Tamanho em bytes de cada elemento (apenas para arrays primitivos). 
   * Ex: 4 para int, 8 para double.
   */
  size_t elem_size;
  /** @brief Armazenamento contíguo para tipos primitivos (byte, int, long, etc). */
  std::vector<u1> raw;

  // Para arrays de referência, usamos refs.
  /** @brief Armazenamento para arrays de referência (ponteiros para RuntimeObject). */
  std::vector<RuntimeObject *> refs;

  /** @brief Construtor padrão privado (use os métodos estáticos create_*). */
  RuntimeArray()
      : component_class(nullptr), is_reference(false), length(0), elem_size(0) {
  }

  /**
   * @brief Cria um array de tipos primitivos.
   * @param component Classe do componente (ex: int.class).
   * @param len Quantidade de elementos.
   * @param elem_sz Tamanho em bytes de cada elemento.
   * @return Ponteiro para o novo RuntimeArray alocado.
   */
  static RuntimeArray *create_primitive(RuntimeClass *component, size_t len,
                                        size_t elem_sz) {
    auto *arr = new RuntimeArray();
    arr->component_class = component;
    arr->is_reference = false;
    arr->length = len;
    arr->elem_size = elem_sz;
    arr->raw.resize(len * elem_sz);
    return arr;
  }

  /**
   * @brief Cria um array de referências (objetos).
   * @param component Classe do componente (ex: String.class).
   * @param len Quantidade de elementos.
   * @return Ponteiro para o novo RuntimeArray alocado.
   */
  static RuntimeArray *create_reference(RuntimeClass *component, size_t len) {
    auto *arr = new RuntimeArray();
    arr->component_class = component;
    arr->is_reference = true;
    arr->length = len;
    arr->elem_size = sizeof(RuntimeObject *);
    arr->refs.resize(len, nullptr);
    return arr;
  }

  /**
   * @brief Verifica se o índice está dentro dos limites.
   * @throws std::runtime_error Se o índice for inválido (ArrayIndexOutOfBounds).
   */
  void check_index(size_t idx) const {
    if (idx >= length) {
      throw std::runtime_error("Array index out of bounds");
    }
  }
  /**
   * @brief Lê um valor primitivo do array.
   * @tparam T Tipo do dado a ler (int, float, etc).
   */

  template <typename T> T read_primitive(size_t idx) const {
    check_index(idx);
    if (is_reference)
      throw std::runtime_error("Attempt to read primitive from ref array");
    T value{};
    std::memcpy(&value, &raw[idx * elem_size], sizeof(T));
    return value;
  }
  /**
   * @brief Escreve um valor primitivo no array.
   * @tparam T Tipo do dado a escrever.
   */

  template <typename T> void write_primitive(size_t idx, T value) {
    check_index(idx);
    if (is_reference)
      throw std::runtime_error("Attempt to write primitive to ref array");
    std::memcpy(&raw[idx * elem_size], &value, sizeof(T));
  }

  /** @brief Lê uma referência de objeto do array. */
  RuntimeObject *read_ref(size_t idx) const {
    check_index(idx);
    if (!is_reference)
      throw std::runtime_error("Attempt to read ref from primitive array");
    return refs[idx];
  }

  /** @brief Escreve uma referência de objeto no array. */
  void write_ref(size_t idx, RuntimeObject *ref) {
    check_index(idx);
    if (!is_reference)
      throw std::runtime_error("Attempt to write ref to primitive array");
    refs[idx] = ref;
  }
};
