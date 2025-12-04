#pragma once

#include "./runtime_class_types.h"
#include <cstddef>
#include <stdexcept>
#include <vector>

class RuntimeArray {
public:
  RuntimeClass *component_class;
  bool is_reference;
  size_t length;

  // Para arrays de primitivos, os bytes ficam em raw com passo elem_size.
  size_t elem_size;
  std::vector<u1> raw;

  // Para arrays de referência, usamos refs.
  std::vector<RuntimeObject *> refs;

  RuntimeArray()
      : component_class(nullptr), is_reference(false), length(0), elem_size(0) {
  }

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

  static RuntimeArray *create_reference(RuntimeClass *component, size_t len) {
    auto *arr = new RuntimeArray();
    arr->component_class = component;
    arr->is_reference = true;
    arr->length = len;
    arr->elem_size = sizeof(RuntimeObject *);
    arr->refs.resize(len, nullptr);
    return arr;
  }

  void check_index(size_t idx) const {
    if (idx >= length) {
      throw std::runtime_error("Array index out of bounds");
    }
  }

  template <typename T> T read_primitive(size_t idx) const {
    check_index(idx);
    if (is_reference)
      throw std::runtime_error("Attempt to read primitive from ref array");
    T value{};
    std::memcpy(&value, &raw[idx * elem_size], sizeof(T));
    return value;
  }

  template <typename T> void write_primitive(size_t idx, T value) {
    check_index(idx);
    if (is_reference)
      throw std::runtime_error("Attempt to write primitive to ref array");
    std::memcpy(&raw[idx * elem_size], &value, sizeof(T));
  }

  RuntimeObject *read_ref(size_t idx) const {
    check_index(idx);
    if (!is_reference)
      throw std::runtime_error("Attempt to read ref from primitive array");
    return refs[idx];
  }

  void write_ref(size_t idx, RuntimeObject *ref) {
    check_index(idx);
    if (!is_reference)
      throw std::runtime_error("Attempt to write ref to primitive array");
    refs[idx] = ref;
  }
};
