#pragma once

#include "../classfile/classfile_types.h"
#include "./debug.h"
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Estruturas do runtime
class RuntimeClass;
class ClassLoader;
class Runtime;
class Interpreter;
class Thread;
class RuntimeObject;
// Avisa para o compilador que ao chamar essa função, o controle não irá
// retornar para a função chamadora
[[noreturn]] void throw_java_exception(RuntimeObject *exception_obj);

// RuntimeField e RuntimeMethod

static const std::unordered_map<char, u4> descriptor_table = {
    {'B', 1}, // byte
    {'Z', 1}, // boolean
    {'C', 2}, // char
    {'S', 2}, // short
    {'I', 4}, // int
    {'F', 4}, // float
    {'L', 4}, // reference
    {'[', 4}, // array reference
    {'J', 8}, // long
    {'D', 8}, // double
};

struct RuntimeField {
  std::string name;
  std::string descriptor;
  u2 access_flags;
  bool is_static;
  bool is_64bit;
  u4 static_offset;
  class RuntimeClass *owner;

  // Offset em bytes ou slots (dependendo do modelo)
  u4 offset;

  // Valor estático armazenado como bytes
  std::vector<u1> static_data;

  RuntimeField()
      : access_flags(0), is_static(false), is_64bit(false), static_offset(0),
        owner(nullptr), offset(0) {}

  u4 size_in_bytes() const {
    if (descriptor.empty())
      return 4;

    auto it = descriptor_table.find(descriptor[0]);
    if (it != descriptor_table.end()) {
      return it->second;
    }

    return 4;
  }
};

struct RuntimeMethod {
  std::string name;
  std::string descriptor;
  u2 access_flags;
  const CodeAttribute *code; // aponta diretamente para o atributo do ClassFile
  int arg_slots;
  RuntimeClass *owner;

  RuntimeMethod()
      : access_flags(0), code(nullptr), arg_slots(0), owner(nullptr) {}
};

// ------------------------------------------------------
// 2. RuntimeClass
// ------------------------------------------------------

class RuntimeClass {
public:
  std::string name;
  std::string super_name;
  u2 access_flags;
  std::vector<u1> static_data;

  RuntimeClass *super_class;
  std::unique_ptr<ClassFile> class_file;

  std::unordered_map<std::string, RuntimeField> fields;
  std::unordered_map<std::string, RuntimeMethod> methods;

  RuntimeClass()
      : name(), super_name(), access_flags(0), super_class(nullptr),
        class_file(nullptr), fields(), methods() {}

  // Busca de método/field
  RuntimeMethod *find_method(const std::string &name,
                             const std::string &descriptor);
  RuntimeField *find_field(const std::string &name,
                           const std::string &descriptor);

  // Tamanho em bytes do data
  u4 data_size();
};

// Objetos

struct RuntimeObject {
  RuntimeClass *klass;
  std::vector<u1> data; // bytes da instância

  RuntimeObject(RuntimeClass *k) : klass(k) { data.resize(k->data_size()); }

  template <typename T> T read_field(const RuntimeField &field) const {
    T value;
    std::memcpy(&value, &data[field.offset], sizeof(T));
    return value;
  }

  template <typename T> void write_field(const RuntimeField &field, T value) {
    std::memcpy(&data[field.offset], &value, sizeof(T));
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

  Frame(RuntimeMethod *method, RuntimeClass *current_class)
      : method(method), current_class(current_class), pc(0) {}

  void init(u4 max_locals, u4 max_stack) {
    local_vars.resize(max_locals);
    operand_stack.stack.reserve(max_stack);
  }
};

struct Thread {
  std::vector<Frame *> call_stack;
  Frame &current_frame() { return *call_stack.back(); }
  Runtime *runtime;
  Interpreter *interpreter;

  Thread(Runtime *rt);
  ~Thread();

  void push_frame(Frame *f) {
    call_stack.push_back(f);

    if (!f) {
      DEBUG_LOG("[JVM] Push frame -> <null> | depth=" << call_stack.size());
      return;
    }

    std::string class_name =
        f->current_class ? f->current_class->name : "<unknown-class>";
    std::string method_name = f->method ? f->method->name : "<unknown-method>";
    std::string descriptor =
        f->method ? (" " + f->method->descriptor) : std::string{};

    DEBUG_LOG("[JVM] Push frame -> " << class_name << "." << method_name
              << descriptor << " | depth=" << call_stack.size());
  }
  void pop_frame() {
    if (call_stack.empty()) {
      if (g_debug_enabled)
        std::cerr
            << "[JVM] Pop frame requested but call stack is already empty\n";
      return;
    }

    Frame *top = call_stack.back();

    std::string class_name =
        top && top->current_class ? top->current_class->name : "<unknown-class>";
    std::string method_name =
        top && top->method ? top->method->name : "<unknown-method>";
    std::string descriptor =
        top && top->method ? (" " + top->method->descriptor) : std::string{};

    DEBUG_LOG("[JVM] Pop frame <- " << class_name << "." << method_name
              << descriptor << " | depth=" << (call_stack.size() - 1));

    call_stack.pop_back();
    delete top;
  }
  void return_frame() {}

  void execute_stack();
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
  explicit BootstrapClassLoader(const std::string classpath, Runtime *runtime)
      : classpath_(classpath), runtime(runtime) {}

  RuntimeClass *load_class(const std::string &name) override;
  void set_classpath(std::string path) { classpath_ = std::move(path); }

private:
  std::string classpath_;
  std::unordered_map<std::string, std::unique_ptr<RuntimeClass>> loaded_;

  std::unique_ptr<RuntimeClass>
  build_runtime_class(std::unique_ptr<ClassFile> cf);

  Runtime *runtime;
};

// Interpretador (esqueleto)
struct Interpreter {
  Thread *thread;
  using Opfunc = void (*)(Thread *, Frame &);
  std::unordered_map<uint8_t, Opfunc> opcode_table = {};
  Interpreter(Thread *t);

  void execute(Frame &frame);
};

// Method Area
class MethodArea {
private:
  std::unordered_map<std::string, std::unique_ptr<RuntimeClass>> classes;

public:
  RuntimeClass *getClassRef(const std::string &name);
  void storeClass(std::unique_ptr<RuntimeClass> klass);
};

// Runtime
// Pensei mais como um classe para resolver o início do modo interpretador e
// reunir os componentes para organizar melhor as classes
class Runtime {
public:
  Thread *thread;
  MethodArea *method_area;

  ClassLoader *class_loader;

  Runtime() {
    thread = new Thread(this);
    method_area = new MethodArea();
    class_loader = new BootstrapClassLoader({}, this);
  }

  ~Runtime();
  void start(std::string filepath, const std::vector<std::string> &args);
};
