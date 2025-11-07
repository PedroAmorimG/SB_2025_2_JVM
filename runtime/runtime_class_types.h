#pragma once

#include "classfile_types.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Estruturas do runtime

struct RuntimeClass;
struct ClassLoader;
struct RuntimeObject;

// RuntimeField e RuntimeMethod

struct RuntimeField {
  std::string name;
  std::string descriptor;
  u2 access_flags;
  bool is_static;

  // Offset em bytes ou slots (dependendo do modelo)
  u4 offset;

  // Valor estático armazenado como bytes
  std::vector<u1> static_data;

  RuntimeField() : access_flags(0), is_static(false), offset(0) {}

  // Retorna o tamanho (em bytes) do campo, baseado no descriptor
  u4 size_in_bytes() const {
    switch (descriptor[0]) {
    case 'B': // byte
    case 'Z': // boolean
      return 1;
    case 'C': // char
    case 'S': // short
      return 2;
    case 'I': // int
    case 'F': // float
    case 'L': // reference (4 bytes)
    case '[': // array reference
      return 4;
    case 'J': // long
    case 'D': // double
      return 8;
    default:
      return 4; // fallback
    }
  }
};

struct RuntimeMethod {
  std::string name;
  std::string descriptor;
  u2 access_flags;
  const CodeAttribute *code; // aponta diretamente para o atributo do ClassFile

  RuntimeMethod() : access_flags(0), code(nullptr) {}
};

// ------------------------------------------------------
// 2. RuntimeClass
// ------------------------------------------------------

struct RuntimeClass {
  std::string name;
  std::string super_name;
  u2 access_flags;

  RuntimeClass *super_class;
  ClassFile *class_file;
  ClassLoader *defining_loader;
  bool initialized;

  std::vector<RuntimeField> fields;
  std::vector<RuntimeMethod> methods;

  RuntimeClass()
      : access_flags(0), super_class(nullptr), class_file(nullptr),
        defining_loader(nullptr), initialized(false) {}

  // Busca de método/field
  RuntimeMethod *find_method(const std::string &name,
                             const std::string &descriptor);
  RuntimeField *find_field(const std::string &name,
                           const std::string &descriptor);
};

// Objetos

struct RuntimeObject {
  RuntimeClass *klass;
  std::vector<u1> data; // bytes da instância

  RuntimeObject(RuntimeClass *k) : klass(k) {
    // calcula o tamanho total dos campos de instância
    u4 total_size = 0;
    for (auto &f : k->fields) {
      if (!f.is_static)
        total_size += f.size_in_bytes();
    }
    data.resize(total_size);
  }

  // Leitura genérica
  template <typename T> T read_field(const RuntimeField &field) const {
    return *(T *)&data[field.offset];
  }

  // Escrita genérica
  template <typename T> void write_field(const RuntimeField &field, T value) {
    *(T *)&data[field.offset] = value;
  }
};

// Frame e pilha de execução

using Slot = u4;

struct OperandStack {
  std::vector<Slot> stack;

  // --- Push de 32 bits ---
  void push_int(int32_t v) { stack.push_back(static_cast<u4>(v)); }

  // --- Push de 64 bits (2 slots) ---
  void push_long(int64_t v) {
    u4 high = static_cast<u4>((v >> 32) & 0xFFFFFFFF);
    u4 low = static_cast<u4>(v & 0xFFFFFFFF);
    stack.push_back(high);
    stack.push_back(low);
  }

  // --- Pop de 32 bits ---
  int32_t pop_int() {
    if (stack.empty())
      throw std::runtime_error("Operand stack underflow");
    int32_t v = static_cast<int32_t>(stack.back());
    stack.pop_back();
    return v;
  }

  // --- Pop de 64 bits ---
  int64_t pop_long() {
    if (stack.size() < 2)
      throw std::runtime_error("Operand stack underflow");
    u4 low = stack.back();
    stack.pop_back();
    u4 high = stack.back();
    stack.pop_back();
    return (static_cast<int64_t>(high) << 32) | low;
  }

  // --- Referências ---
  void push_ref(RuntimeObject *ref) {
    // armazenar ponteiro como índice ou endereço truncado (32-bit)
    stack.push_back(static_cast<u4>(reinterpret_cast<uintptr_t>(ref)));
  }

  RuntimeObject *pop_ref() {
    if (stack.empty())
      throw std::runtime_error("Operand stack underflow");
    u4 bits = stack.back();
    stack.pop_back();
    return reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(bits));
  }

  // --- Float / Double helpers ---
  void push_float(float v) {
    union {
      float f;
      u4 u;
    } conv;
    conv.f = v;
    stack.push_back(conv.u);
  }

  float pop_float() {
    if (stack.empty())
      throw std::runtime_error("Operand stack underflow");
    union {
      float f;
      u4 u;
    } conv;
    conv.u = stack.back();
    stack.pop_back();
    return conv.f;
  }

  void push_double(double v) {
    union {
      double d;
      u8 u;
    } conv;
    conv.d = v;
    push_long(static_cast<int64_t>(conv.u));
  }

  double pop_double() {
    union {
      double d;
      u8 u;
    } conv;
    conv.u = static_cast<u8>(pop_long());
    return conv.d;
  }

  bool empty() const { return stack.empty(); }
  size_t size() const { return stack.size(); }
};

// =====================================================
// Frame: contexto de execução de um método
// =====================================================
struct Frame {
  RuntimeMethod *method;
  RuntimeClass *current_class;
  std::vector<Slot> local_vars;
  OperandStack operand_stack;
  u4 pc;

  Frame() : method(nullptr), current_class(nullptr), pc(0) {}

  void init(u4 max_locals, u4 max_stack) {
    local_vars.resize(max_locals);
    operand_stack.stack.reserve(max_stack);
  }
};

struct Thread {
  std::vector<Frame> call_stack;
  Frame &current_frame() { return call_stack.back(); }
};

//  ClassLoader base

class ClassLoader {
public:
  virtual RuntimeClass *load_class(const std::string &name) = 0;
  virtual ~ClassLoader() {}
};

//  BootstrapClassLoader

class BootstrapClassLoader : public ClassLoader {
public:
  explicit BootstrapClassLoader(const std::vector<std::string> &classpath)
      : classpath_(classpath) {}

  RuntimeClass *load_class(const std::string &name) override;

private:
  std::vector<std::string> classpath_;
  std::unordered_map<std::string, std::unique_ptr<RuntimeClass>> loaded_;

  std::vector<u1> read_class_bytes(const std::string &name);
  ClassFile parse_class_file(const std::vector<u1> &bytes);
  std::unique_ptr<RuntimeClass> build_runtime_class(ClassFile *cf);
  void link_class(RuntimeClass *klass);
};

// Interpretador (esqueleto)

struct Interpreter {
  void execute(Frame &frame);
};
