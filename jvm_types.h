#ifndef JVM_TYPES_H
#define JVM_TYPES_H

#include <stdint.h>
#include <variant>
#include <vector>

// Tipos básicos
using u1 = uint8_t;
using u2 = uint16_t;
using u4 = uint32_t;
using u8 = uint64_t;
static_assert(sizeof(u1) == 1 && sizeof(u2) == 2 && sizeof(u4) == 4 &&
                  sizeof(u8) == 8,
              "Fixed-width integer sizes required");

// Tags do constant pool
enum ConstantTag : u1 {
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

// Estruturas do constant pool

struct ConstantClassInfo {
  u2 name_index;
};

struct ConstantFieldrefInfo {
  u2 class_index;
  u2 name_and_type_index;
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
  u2 name_index;
  u2 descriptor_index;
};

struct ConstantUTF8Info {
  u2 length;
  std::vector<u1> bytes;
};

struct ConstantStringInfo {
  u2 string_index;
};

struct ConstantIntegerInfo {
  u4 bytes;
};

struct ConstantFloatInfo {
  u4 bytes;
};

struct ConstantLongInfo {
  u4 high_bytes;
  u4 low_bytes;
};

struct ConstantDoubleInfo {
  u4 high_bytes;
  u4 low_bytes;
};

struct EmptyInfo // Índice 0 do pool de constantes
{};

using ConstantInfo =
    std::variant<EmptyInfo, ConstantClassInfo, ConstantFieldrefInfo,
                 ConstantMethodrefInfo, ConstantInterfaceMethodrefInfo,
                 ConstantStringInfo, ConstantIntegerInfo, ConstantFloatInfo,
                 ConstantLongInfo, ConstantDoubleInfo, ConstantNameAndTypeInfo,
                 ConstantUTF8Info>;

using ConstantPoolEntry = std::pair<ConstantTag, ConstantInfo>;

// FieldInfo

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

// AttributeInfo

struct AttributeInfo {
  u2 attribute_name_index;
  u4 attribute_length;
  std::vector<u1> info;
};

struct FieldInfo {
  FieldAccessFlag access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  std::vector<AttributeInfo> attributes;
};

// MethodInfo

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
};

// ClassFile
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
};

#endif
