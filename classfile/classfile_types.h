#pragma once

#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

// Tipos básicos com tamanho garantido (u1=1 byte, u2=2 bytes, etc.)
using u1 = uint8_t;
using u2 = uint16_t;
using u4 = uint32_t;
using u8 = uint64_t;
static_assert(sizeof(u1) == 1 && sizeof(u2) == 2 && sizeof(u4) == 4 &&
                  sizeof(u8) == 8,
              "Fixed-width integer sizes required");

/**
 * @enum ConstantTag
 * @brief Identificadores (tags) para os tipos de constantes no Constant Pool.
 */
enum class ConstantTag : u1 {
  CONSTANT_Class = 7,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_String = 8,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_NameAndType = 12,
  CONSTANT_Utf8 = 1,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_InvokeDynamic = 18,
  None = 0,
};

//  Estruturas do Constant Pool 

struct ConstantClassInfo {
  u2 name_index; ///< Índice para um CONSTANT_Utf8 com o nome da classe.
};

struct ConstantFieldrefInfo {
  u2 class_index;       ///< Índice para a classe que contém o campo.
  u2 name_and_type_index; ///< Índice para NameAndType.
};

struct ConstantMethodrefInfo {
  u2 class_index;
  u2 name_and_type_index;
};

struct ConstantInterfaceMethodrefInfo {
  u2 class_index;
  u2 name_and_type_index;
};

struct ConstantNameAndTypeInfo {
  u2 name_index;       ///< Nome do campo/método.
  u2 descriptor_index; ///< Descritor de tipo (ex: "()V").
};

struct ConstantUTF8Info {
  u2 length;
  u1 *bytes; ///< Array de bytes da string (não null-terminated).
};

struct ConstantStringInfo {
  u2 string_index; ///< Índice para o Utf8 com o valor da string.
};

struct ConstantIntegerInfo {
  u4 bytes; ///< Valor inteiro de 4 bytes (Big-Endian).
};

struct ConstantFloatInfo {
  u4 bytes; ///< Valor float em formato IEEE 754 (bits brutos).
};

struct ConstantLongInfo {
  u4 high_bytes;
  u4 low_bytes;
};

struct ConstantDoubleInfo {
  u4 high_bytes;
  u4 low_bytes;
};

struct EmptyInfo {}; ///< Placeholder para slots vazios ou padding.

/**
 * @union ConstantInfo
 * @brief União que armazena o valor de uma constante, dependendo da sua Tag.
 */
union ConstantInfo {
  EmptyInfo empty;
  ConstantClassInfo class_info;
  ConstantFieldrefInfo fieldref_info;
  ConstantMethodrefInfo methodref_info;
  ConstantInterfaceMethodrefInfo interface_methodref_info;
  ConstantStringInfo string_info;
  ConstantIntegerInfo integer_info;
  ConstantFloatInfo float_info;
  ConstantLongInfo long_info;
  ConstantDoubleInfo double_info;
  ConstantNameAndTypeInfo name_and_type_info;
  ConstantUTF8Info utf8_info;
};

using ConstantPoolEntry = std::pair<ConstantTag, ConstantInfo>;

//  Fields 

enum FieldAccessFlag : u2 {
  ACC_Public_Field = 0x0001,
  ACC_Private_Field = 0x0002,
  ACC_Protected_Field = 0x0004,
  ACC_Static_Field = 0x0008,
  ACC_Final_Field = 0x0010,
  ACC_Volatile_Field = 0x0040,
  ACC_Transient_Field = 0x0080,
  ACC_Synthetic_Field = 0x1000,
  ACC_Enum_Field = 0x4000,
};

//  Atributos

struct AttributeInfo; // Forward declaration

struct ExceptionTableEntry {
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  u2 catch_type;
};

/**
 * @struct CodeAttribute
 * @brief O atributo mais importante, contendo o bytecode do método.
 */
struct CodeAttribute {
  u2 max_stack;   ///< Tamanho máximo da pilha de operandos.
  u2 max_locals;  ///< Tamanho máximo do vetor de variáveis locais.
  u4 code_length;
  std::vector<u1> code; ///< O array de bytecodes (instruções).
  u2 exception_table_length;
  std::vector<ExceptionTableEntry> exception_table;
  u2 attributes_count;
  std::vector<AttributeInfo> attributes; ///< Atributos aninhados (ex: LineNumberTable).
};

struct LineNumberTableEntry {
  u2 start_pc;    ///< Índice no bytecode.
  u2 line_number; ///< Linha correspondente no código fonte original.
};

struct LineNumberTableAttribute {
  u2 attribute_name_index;
  u4 attribute_length;
  u2 line_number_table_length;
  std::vector<LineNumberTableEntry> line_number_table;
};

struct SourceFileAttribute {
  u2 sourcefile_index;
};

struct UnknownAttribute {
  std::vector<u1> info; ///< Bytes brutos de atributos não suportados.
};

struct ConstantValueAttribute {
  u2 constantvalue_index;
};

struct SyntheticAttribute {};

struct ExceptionsAttribute {
  u2 number_of_exceptions;
  std::vector<u2> exception_index_table;
};

struct InnerClassInfo {
  u2 inner_class_info_index;
  u2 outer_class_info_index;
  u2 inner_name_index;
  u2 inner_class_access_flags;
};

struct InnerClassesAttribute {
  u2 number_of_classes;
  std::vector<InnerClassInfo> classes;
};

// StackMapTable (Verificação de Tipos) 

enum class VTTag : u1 {
  Top = 0, Integer = 1, Float = 2, Double = 3, Long = 4,
  Null = 5, UninitializedThis = 6, Object = 7, Uninitialized = 8
};

struct VerificationTypeInfo {
  VTTag tag;
  u2 cpool_index = 0;
  u2 offset = 0;
};

enum class SMFKind : u1 {
  Same, SameLocals1StackItem, SameLocals1StackItemExt,
  Chop, SameExt, Append, Full
};

struct StackMapFrame {
  SMFKind kind;
  u1 frame_type;
  u2 offset_delta = 0;
  VerificationTypeInfo stack_item;
  std::vector<VerificationTypeInfo> locals_appended;
  std::vector<VerificationTypeInfo> locals_full;
  std::vector<VerificationTypeInfo> stack_full;
};

struct StackMapTableInfo {
  u2 number_of_entries = 0;
  std::vector<StackMapFrame> entries;
};

struct LocalVariableTableEntry {
  u2 start_pc; u2 length; u2 name_index; u2 descriptor_index; u2 index;
};

struct LocalVariableTableInfo {
  u2 local_variable_table_length;
  std::vector<LocalVariableTableEntry> local_variable_table;
};

/**
 * @struct AttributeInfo
 * @brief Estrutura genérica que encapsula qualquer tipo de atributo.
 * O parser preenche o campo específico baseado no `attribute_name`.
 */
struct AttributeInfo {
  u2 attribute_name_index;
  std::string attribute_name;
  u4 attribute_length;

  CodeAttribute code_info;
  SourceFileAttribute sourcefile_info;
  ConstantValueAttribute constantvalue_info;
  SyntheticAttribute synthetic_info;
  LineNumberTableAttribute linenumbertable_info;
  StackMapTableInfo stackmaptable_info;
  LocalVariableTableInfo localvariabletable_info;
  UnknownAttribute unknown_info;
  ExceptionsAttribute exceptions_info;
  InnerClassesAttribute innerclasses_info;
};

struct FieldInfo {
  FieldAccessFlag access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  std::vector<AttributeInfo> attributes;
};

//  Methods 

enum MethodAccessFlag : u2 {
  ACC_Public_Method = 0x0001,
  ACC_Private_Method = 0x0002,
  ACC_Protected_Method = 0x0004,
  ACC_Static_Method = 0x0008,
  ACC_Final_Method = 0x0010,
  ACC_Synchronized_Method = 0x0020,
  ACC_Bridge_Method = 0x0040,
  ACC_Varargs_Method = 0x0080,
  ACC_Native_Method = 0x0100,
  ACC_Abstract_Method = 0x0400,
  ACC_Strict_Method = 0x0800,
  ACC_Synthetic_Method = 0x1000,
};

struct MethodInfo {
  MethodAccessFlag access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  std::vector<AttributeInfo> attributes;

  /**
   * @brief Helper para encontrar o atributo "Code" deste método.
   * @return Um ponteiro para o CodeAttribute ou nullptr se não existir.
   */
  const CodeAttribute *find_code_attribute() const {
    for (const auto &attr : attributes) {
      if (attr.attribute_name == "Code") {
        return &attr.code_info;
      }
    }
    return nullptr;
  }
};

//  ClassFile Principal 

/**
 * @struct ClassFile
 * @brief Representação completa de um arquivo .class parseado em memória.
 */
struct ClassFile {
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
  std::vector<ConstantPoolEntry> constant_pool;
  u2 access_flags;
  u2 this_class;
  u2 super_class;
  u2 interfaces_count;
  std::vector<u2> interfaces;
  u2 fields_count;
  std::vector<FieldInfo> fields;
  u2 methods_count;
  std::vector<MethodInfo> methods;
  u2 attributes_count;
  std::vector<AttributeInfo> attributes;

  /**
   
   * @brief Resolve um índice da Constant Pool para sua representação em String.
   * * Esta função é recursiva. Se o índice apontar para uma ClassInfo, ela busca o
   * nome da classe. Se apontar para NameAndType, busca o nome e descritor.
   * * @param index O índice na constant pool a ser resolvido.
   * @return A string resolvida (ex: "java/lang/Object") ou vazia em caso de erro.
   */
  
  std::string resolve_utf8(u2 index) const {
    if (index == 0 || index >= constant_pool.size()) return "";
    const auto &entry = constant_pool[index];

    switch (entry.first) {
    case ConstantTag::CONSTANT_Class:
      return resolve_utf8(entry.second.class_info.name_index);
    case ConstantTag::CONSTANT_Fieldref:
      return resolve_utf8(entry.second.fieldref_info.name_and_type_index);
    case ConstantTag::CONSTANT_Methodref:
      return resolve_utf8(entry.second.methodref_info.name_and_type_index);
    case ConstantTag::CONSTANT_InterfaceMethodref:
      return resolve_utf8(entry.second.interface_methodref_info.name_and_type_index);
    case ConstantTag::CONSTANT_NameAndType:
      return resolve_utf8(entry.second.name_and_type_info.descriptor_index) +
             " " + resolve_utf8(entry.second.name_and_type_info.name_index);
    case ConstantTag::CONSTANT_Utf8: {
      const ConstantUTF8Info &utf = entry.second.utf8_info;
      return std::string(reinterpret_cast<const char *>(utf.bytes), utf.length);
    }
    default: return "";
    }
    return "";
  }
};