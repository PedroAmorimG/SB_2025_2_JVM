#include "native_methods.h"

#include <cstring>
#include <iostream>
#include <string>

// Chaves para mapear métodos nativos.
// Formato: "<descriptor> <class>/<method>"
static const std::string PRINTLN_VOID_KEY = "()V java/io/PrintStream.println";
static const std::string PRINTLN_STR_KEY =
    "(Ljava/lang/String;)V java/io/PrintStream.println";
static const std::string PRINTLN_INT_KEY = "(I)V java/io/PrintStream.println";
static const std::string PRINTLN_LONG_KEY = "(J)V java/io/PrintStream.println";
static const std::string PRINTLN_BOOL_KEY = "(Z)V java/io/PrintStream.println";
static const std::string PRINTLN_FLOAT_KEY = "(F)V java/io/PrintStream.println";
static const std::string PRINTLN_DOUBLE_KEY =
    "(D)V java/io/PrintStream.println";

static const std::string STRING_LENGTH_KEY = "()I java/lang/String.length";
static const std::string STRING_CHAR_AT_KEY = "(I)C java/lang/String.charAt";
static const std::string STRING_EQUALS_KEY =
    "(Ljava/lang/Object;)Z java/lang/String.equals";
static const std::string STRING_GET_BYTES_KEY =
    "()[B java/lang/String.getBytes";

// Helpers para manipular o campo value da String.
static RuntimeArray *get_string_value(RuntimeObject *str_obj) {
  if (str_obj == nullptr || str_obj->klass == nullptr)
    return nullptr;

  auto field = str_obj->klass->find_field("value", "[B");
  if (!field)
    return nullptr;

  u4 ref_bits = str_obj->read_field<u4>(*field);
  return reinterpret_cast<RuntimeArray *>(static_cast<uintptr_t>(ref_bits));
}

static std::string string_from_value(RuntimeObject *str_obj) {
  auto arr = get_string_value(str_obj);
  if (!arr || arr->length == 0 || arr->is_reference)
    return "";

  return std::string(reinterpret_cast<const char *>(arr->raw.data()),
                     arr->length);
}

void print_ln(Frame &frame) { std::cout << std::endl; }

void print_str_ln(Frame &frame) {
  auto ref_bits = frame.local_vars.size() > 1 ? frame.local_vars[1] : 0;
  auto str_obj =
      reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(ref_bits));
  std::cout << string_from_value(str_obj) << std::endl;
}

void print_int_ln(Frame &frame) {
  int32_t value = static_cast<int32_t>(
      frame.local_vars.size() > 1 ? frame.local_vars[1] : 0);
  std::cout << value << std::endl;
}

void print_lg_ln(Frame &frame) {
  int64_t value = 0;
  if (frame.local_vars.size() > 2) {
    u4 high = frame.local_vars[1];
    u4 low = frame.local_vars[2];
    value = (static_cast<int64_t>(high) << 32) | low;
  }
  std::cout << value << std::endl;
}

void print_bl_ln(Frame &frame) {
  int32_t value = static_cast<int32_t>(
      frame.local_vars.size() > 1 ? frame.local_vars[1] : 0);
  std::cout << (value != 0 ? "true" : "false") << std::endl;
}

void print_fl_ln(Frame &frame) {
  float value = 0.0f;
  if (frame.local_vars.size() > 1) {
    u4 bits = frame.local_vars[1];
    std::memcpy(&value, &bits, sizeof(float));
  }
  std::cout << value << std::endl;
}

void print_db_ln(Frame &frame) {
  double value = 0.0;
  if (frame.local_vars.size() > 2) {
    u4 high = frame.local_vars[1];
    u4 low = frame.local_vars[2];
    uint64_t bits = (static_cast<uint64_t>(high) << 32) | low;
    std::memcpy(&value, &bits, sizeof(double));
  }
  std::cout << value << std::endl;
}

void str_len(Frame &frame) {
  auto self_bits = frame.local_vars.size() > 0 ? frame.local_vars[0] : 0;
  auto str_obj =
      reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(self_bits));
  auto arr = get_string_value(str_obj);
  int len = static_cast<int>(arr ? arr->length : 0);
  frame.operand_stack.push_int(len);
}

void str_char_at(Frame &frame) {
  auto self_bits = frame.local_vars.size() > 0 ? frame.local_vars[0] : 0;
  auto str_obj =
      reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(self_bits));
  auto arr = get_string_value(str_obj);

  int idx =
      static_cast<int>(frame.local_vars.size() > 1 ? frame.local_vars[1] : 0);
  if (!arr)
    throw std::runtime_error("Null string on charAt");
  if (idx < 0 || static_cast<size_t>(idx) >= arr->length)
    throw std::runtime_error("String index out of bounds");

  uint16_t ch = arr->read_primitive<uint8_t>(static_cast<size_t>(idx));
  frame.operand_stack.push_int(ch); // char promovido para int
}

void str_eq(Frame &frame) {
  auto self_bits = frame.local_vars.size() > 0 ? frame.local_vars[0] : 0;
  auto other_bits = frame.local_vars.size() > 1 ? frame.local_vars[1] : 0;
  auto self_obj =
      reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(self_bits));
  auto other_obj =
      reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(other_bits));

  if (self_obj == other_obj) {
    frame.operand_stack.push_int(1);
    return;
  }

  auto self_arr = get_string_value(self_obj);
  auto other_arr = get_string_value(other_obj);
  if (!self_arr || !other_arr || self_arr->is_reference ||
      other_arr->is_reference || self_arr->length != other_arr->length) {
    frame.operand_stack.push_int(0);
    return;
  }

  bool eq = std::memcmp(self_arr->raw.data(), other_arr->raw.data(),
                        self_arr->length) == 0;
  frame.operand_stack.push_int(eq ? 1 : 0);
}

void str_get_bytes(Frame &frame) {
  auto self_bits = frame.local_vars.size() > 0 ? frame.local_vars[0] : 0;
  auto str_obj =
      reinterpret_cast<RuntimeObject *>(static_cast<uintptr_t>(self_bits));
  auto arr = get_string_value(str_obj);
  frame.operand_stack.push_ref(reinterpret_cast<RuntimeObject *>(arr));
}

void load_map(std::unordered_map<std::string, NativeMethod> *native_methods) {
  if (!native_methods)
    return;

  native_methods->emplace(PRINTLN_VOID_KEY, &print_ln);
  native_methods->emplace(PRINTLN_STR_KEY, &print_str_ln);
  native_methods->emplace(PRINTLN_INT_KEY, &print_int_ln);
  native_methods->emplace(PRINTLN_LONG_KEY, &print_lg_ln);
  native_methods->emplace(PRINTLN_BOOL_KEY, &print_bl_ln);
  native_methods->emplace(PRINTLN_FLOAT_KEY, &print_fl_ln);
  native_methods->emplace(PRINTLN_DOUBLE_KEY, &print_db_ln);

  native_methods->emplace(STRING_LENGTH_KEY, &str_len);
  native_methods->emplace(STRING_CHAR_AT_KEY, &str_char_at);
  native_methods->emplace(STRING_EQUALS_KEY, &str_eq);
  native_methods->emplace(STRING_GET_BYTES_KEY, &str_get_bytes);
}
