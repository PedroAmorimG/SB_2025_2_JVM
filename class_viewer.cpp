#include "class_viewer.h"
#include "jvm_types.h"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

ClassFileViewer::ClassFileViewer(ClassFile cf) : cf(cf) {}
void ClassFileViewer::show_class_file() {
  print_magic();
  print_minor_version();
  print_major_version();
  print_java_version();
  print_constant_pool_count();
  print_access_flags();
  print_this_class();
  print_super_class();
  print_interface_count();
  print_interfaces();
  print_field_count();
  print_fields();
}

void ClassFileViewer::print_magic() {
  std::cout << std::hex << std::setfill('0');
  std::cout << "Magic: 0x" << std::setw(8) << cf.magic << std::endl;
  std::cout << std::dec; // volta para decimal
}

void ClassFileViewer::print_major_version() {
  std::cout << "Major version: " << cf.major_version << std::endl;
}

void ClassFileViewer::print_minor_version() {
  std::cout << "Minor version: " << cf.minor_version << std::endl;
}

void ClassFileViewer::print_java_version() {
  std::cout << "Java version: ";
  switch (cf.major_version) {
  case 45:
    std::cout << "1" << std::endl;
    break;
  case 46:
    std::cout << "2" << std::endl;
    break;
  case 47:
    std::cout << "3" << std::endl;
    break;
  case 48:
    std::cout << "4" << std::endl;
    break;
  case 49:
    std::cout << "5" << std::endl;
    break;
  case 50:
    std::cout << "6" << std::endl;
    break;
  case 51:
    std::cout << "7" << std::endl;
    break;
  case 52:
    std::cout << "8" << std::endl;
    break;
  default:
    break;
  }
}

void ClassFileViewer::print_constant_pool_count() {
  std::cout << "Constant pool count: " << cf.constant_pool_count << std::endl;
}

static std::string resolve_utf8(u2 index,
                                const std::vector<ConstantPoolEntry> &cp) {
  if (index < cp.size() && cp[index].first == ConstantTag::CONSTANT_Utf8) {
    const auto &v = cp[index].second.utf8_info;
    return std::string(reinterpret_cast<const char *>(v.bytes), v.length);
  }
  return std::to_string(index);
}

static std::string resolve_class(u2 index,
                                 const std::vector<ConstantPoolEntry> &cp) {
  if (index < cp.size() && cp[index].first == ConstantTag::CONSTANT_Class) {
    const auto &v = cp[index].second.class_info;
    return resolve_utf8(v.name_index, cp);
  }
  return std::to_string(index);
}

static std::string
resolve_name_and_type(u2 index, const std::vector<ConstantPoolEntry> &cp) {
  if (index < cp.size() &&
      cp[index].first == ConstantTag::CONSTANT_NameAndType) {
    const auto &nt = cp[index].second.name_and_type_info;
    return resolve_utf8(nt.name_index, cp) + " " +

           resolve_utf8(nt.descriptor_index, cp);
  }
  return std::to_string(index);
}

// Impressão
void ClassFileViewer::print_constant_entry(u2 index) {
  const ConstantTag tag = cf.constant_pool[index].first;
  const ConstantInfo &info = cf.constant_pool[index].second;

  std::cout << "#" << std::setw(3) << index << " ";

  switch (tag) {
  case ConstantTag::CONSTANT_Class: {
    const auto &v = info.class_info;
    std::cout << "Class\t\tname_index = " << v.name_index << " "
              << resolve_utf8(v.name_index, cf.constant_pool);
    break;
  }

  case ConstantTag::CONSTANT_Fieldref: {
    const auto &v = info.fieldref_info;
    std::cout << "Fieldref\t\tclass_index = " << v.class_index << " "
              << resolve_class(v.class_index, cf.constant_pool) << " "
              << "name_and_type_index = " << v.name_and_type_index << " "
              << resolve_name_and_type(v.name_and_type_index, cf.constant_pool);
    break;
  }

  case ConstantTag::CONSTANT_Methodref: {
    const auto &v = info.methodref_info;
    std::cout << "Methodref\t\tclass_index = " << v.class_index << " "
              << resolve_class(v.class_index, cf.constant_pool) << " "
              << "name_and_type_index = " << v.name_and_type_index << " "
              << resolve_name_and_type(v.name_and_type_index, cf.constant_pool);
    break;
  }

  case ConstantTag::CONSTANT_InterfaceMethodref: {
    const auto &v = info.interface_methodref_info;
    std::cout << "InterfaceMethodref\tclass_index = " << v.class_index << " "
              << resolve_class(v.class_index, cf.constant_pool) << " "
              << "name_and_type_index = " << v.name_and_type_index << " "
              << resolve_name_and_type(v.name_and_type_index, cf.constant_pool);
    break;
  }

  case ConstantTag::CONSTANT_String: {
    const auto &v = info.string_info;
    std::cout << "String\t\tstring_index = " << v.string_index << " "
              << resolve_utf8(v.string_index, cf.constant_pool);
    break;
  }

  case ConstantTag::CONSTANT_Integer: {
    const auto &v = info.integer_info;
    std::cout << "Integer\t\tvalue = " << (int32_t)v.bytes << " "
              << "(hex = 0x" << std::hex << v.bytes << std::dec << ")";
    break;
  }

  case ConstantTag::CONSTANT_Float: {
    const auto &v = info.float_info;
    float f;
    std::memcpy(&f, &v.bytes, sizeof(float));
    std::cout << "Float\t\tvalue = " << f << " (hex = 0x" << std::hex << v.bytes
              << std::dec << ")";
    break;
  }

  case ConstantTag::CONSTANT_Long: {
    const auto &v = info.long_info;
    uint64_t value = (uint64_t(v.high_bytes) << 32) | v.low_bytes;
    std::cout << "Long\t\tvalue = " << value << " (hex = 0x" << std::hex
              << value << std::dec << ")";
    break;
  }

  case ConstantTag::CONSTANT_Double: {
    const auto &v = info.double_info;
    uint64_t bits = (uint64_t(v.high_bytes) << 32) | v.low_bytes;
    double d;
    std::memcpy(&d, &bits, sizeof(double));
    std::cout << "Double\t\tvalue = " << d << " (hex = 0x" << std::hex << bits
              << std::dec << ")";
    break;
  }

  case ConstantTag::CONSTANT_NameAndType: {
    const auto &v = info.name_and_type_info;
    std::cout << "NameAndType\tname_index = " << v.name_index << " "
              << resolve_utf8(v.name_index, cf.constant_pool) << " "
              << "descriptor_index = " << v.descriptor_index << " "
              << resolve_utf8(v.descriptor_index, cf.constant_pool);
    break;
  }

  case ConstantTag::CONSTANT_Utf8: {
    const auto &v = info.utf8_info;
    std::cout << "Utf8\t\t\""
              << std::string(reinterpret_cast<const char *>(v.bytes), v.length)
              << "\" (" << v.length << " bytes)";
    break;
  }

  case ConstantTag::None:
    std::cout << "(empty)";
    break;

  default:
    std::cout << "Tag desconhecido (" << static_cast<int>(tag) << ")";
    break;
  }

  std::cout << "\n";
}

void ClassFileViewer::print_access_flags() {
  std::cout << "Access flags: 0x" << std::hex << cf.access_flags << std::dec
            << " [";

  std::vector<std::string> flags;

  if (cf.access_flags & 0x0001) {
    flags.push_back("ACC_PUBLIC");
  }
  if (cf.access_flags & 0x0010) {
    flags.push_back("ACC_FINAL");
  }
  if (cf.access_flags & 0x0020) {
    flags.push_back("ACC_SUPER");
  }
  if (cf.access_flags & 0x0200) {
    flags.push_back("ACC_INTERFACE");
  }
  if (cf.access_flags & 0x0400) {
    flags.push_back("ACC_ABSTRACT");
  }
  if (cf.access_flags & 0x1000) {
    flags.push_back("ACC_SYNTHETIC");
  }
  if (cf.access_flags & 0x2000) {
    flags.push_back("ACC_ANNOTATION");
  }
  if (cf.access_flags & 0x4000) {
    flags.push_back("ACC_ENUM");
  }

  for (u2 i = 0; i < flags.size(); i++) {

    std::cout << flags[i];

    if (i != flags.size() - 1) {
      std::cout << ", ";
    } else {
      std::cout << "]" << std::endl;
    }
  }
}

void ClassFileViewer::print_this_class() {
  std::cout << "This class index: #" << cf.this_class << " "
            << resolve_class(cf.this_class, cf.constant_pool) << std::endl;
}

void ClassFileViewer::print_super_class() {
  std::cout << "Super class index: #" << cf.super_class << " "
            << resolve_class(cf.super_class, cf.constant_pool) << std::endl;
}

void ClassFileViewer::print_interface_count() {
  std::cout << "Interface count: " << cf.interfaces_count << std::endl;
}

void ClassFileViewer::print_interfaces() {
  for (u2 i = 0; i < cf.interfaces_count; i++) {
    std::cout << "#" << std::setw(3) << i + 1 << " ";
    std::cout << "Class\t\tname_index = " << cf.interfaces[i] << " "
              << resolve_class(cf.interfaces[i], cf.constant_pool) << std::endl;
  }
}

void ClassFileViewer::print_field_count() {
  std::cout << "Field count index: #" << cf.fields_count << std::endl;
}

void ClassFileViewer::print_fields() {
  for (u2 i = 0; i < cf.fields_count; i++) {
    auto entry = cf.fields[i];
    std::cout << "Access flags: 0x" << std::setfill('0') << std::setw(4)
              << std::hex << std::uppercase << entry.access_flags << std::dec
              << " [";
    std::vector<std::string> flags;

    if (entry.access_flags & 0x0001)
      flags.push_back("ACC_PUBLIC");
    if (entry.access_flags & 0x0002)
      flags.push_back("ACC_PRIVATE");
    if (entry.access_flags & 0x0004)
      flags.push_back("ACC_PROTECTED");
    if (entry.access_flags & 0x0008)
      flags.push_back("ACC_STATIC");
    if (entry.access_flags & 0x0010)
      flags.push_back("ACC_FINAL");
    if (entry.access_flags & 0x0040)
      flags.push_back("ACC_VOLATILE");
    if (entry.access_flags & 0x0080)
      flags.push_back("ACC_TRANSIENT");
    if (entry.access_flags & 0x4000)
      flags.push_back("ACC_ENUM");
    if (entry.access_flags & 0x1000)
      flags.push_back("ACC_SYNTHETIC");

    for (u1 i = 0; i < flags.size(); ++i) {
      std::cout << flags[i];
      if (i < flags.size() - 1)
        std::cout << ", ";
    }

    std::cout << "]" << std::endl;

    std::cout << "Name_index = " << entry.name_index << " " << std::endl;
    std::cout << "Descriptor_index = " << entry.descriptor_index << " "
              << std::endl;
    std::cout << "Attribute bytes count = " << entry.attributes_count << " "
              << std::endl;
    print_read_attributes(entry.attributes_count, entry.attributes);
  }
}

void ClassFileViewer::print_attribute_count() {
  std::cout << "Class Attributes count: " << cf.attributes_count << std::endl;
}

void ClassFileViewer::print_code_attribute(const CodeAttribute &code) {
  std::cout << "\t\tCode:" << std::endl;
  std::cout << "\t\t  stack=" << code.max_stack;
  std::cout << ", locals=" << code.max_locals << std::endl;

  std::cout << "\t\t  bytecode (" << code.code_length
            << " bytes):" << std::endl;

  for (u4 i = 0; i < code.code_length;) {
    u1 opcode = code.code[i];
    std::cout << "\t\t    " << i << ": ";

    switch (opcode) {
    case 5: { // iconst_2
      std::cout << "iconst_2";
      i += 1;
      break;
    }
    case 6: { // iconst_3
      std::cout << "iconst_3";
      i += 1;
      break;
    }
    case 7: { // iconst_4
      std::cout << "iconst_4";
      i += 1;
      break;
    }
    case 8: { // iconst_5
      std::cout << "iconst_5";
      i += 1;
      break;
    }
    case 9: { // lconst_0
      std::cout << "lconst_0";
      i += 1;
      break;
    }
    case 10: { // lconst_1
      std::cout << "lconst_1";
      i += 1;
      break;
    }
    case 11: { // fconst_0
      std::cout << "fconst_0";
      i += 1;
      break;
    }
    case 12: { // fconst_1
      std::cout << "fconst_1";
      i += 1;
      break;
    }
    case 14: { // dconst_0
      std::cout << "dconst_0";
      i += 1;
      break;
    }
    case 15: { // dconst_1
      std::cout << "dconst_1";
      i += 1;
      break;
    }
    case 17: { // sipush
      u2 value = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "sipush " << static_cast<int16_t>(value);
      i += 3;
      break;
    }
    case 19: { // ldc_w
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ldc_w #" << index;
      i += 3;
      break;
    }
    case 21: { // iload
      u1 index = code.code[i + 1];
      std::cout << "iload " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 22: { // lload
      u1 index = code.code[i + 1];
      std::cout << "lload " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 23: { // fload
      u1 index = code.code[i + 1];
      std::cout << "fload " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 24: { // dload
      u1 index = code.code[i + 1];
      std::cout << "dload " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 25: { // aload
      u1 index = code.code[i + 1];
      std::cout << "aload " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 27: { // iload_1
      std::cout << "iload_1";
      i += 1;
      break;
    }
    case 29: { // iload_3
      std::cout << "iload_3";
      i += 1;
      break;
    }
    case 30: { // lload_0
      std::cout << "lload_0";
      i += 1;
      break;
    }
    case 31: { // lload_1
      std::cout << "lload_1";
      i += 1;
      break;
    }
    case 32: { // lload_2
      std::cout << "lload_2";
      i += 1;
      break;
    }
    case 33: { // lload_3
      std::cout << "lload_3";
      i += 1;
      break;
    }
    case 34: { // fload_0
      std::cout << "fload_0";
      i += 1;
      break;
    }
    case 35: { // fload_1
      std::cout << "fload_1";
      i += 1;
      break;
    }
    case 36: { // fload_2
      std::cout << "fload_2";
      i += 1;
      break;
    }
    case 37: { // fload_3
      std::cout << "fload_3";
      i += 1;
      break;
    }
    case 38: { // dload_0
      std::cout << "dload_0";
      i += 1;
      break;
    }
    case 39: { // dload_1
      std::cout << "dload_1";
      i += 1;
      break;
    }
    case 40: { // dload_2
      std::cout << "dload_2";
      i += 1;
      break;
    }
    case 41: { // dload_3
      std::cout << "dload_3";
      i += 1;
      break;
    }
    case 46: { // iaload
      std::cout << "iaload";
      i += 1;
      break;
    }
    case 47: { // laload
      std::cout << "laload";
      i += 1;
      break;
    }
    case 48: { // faload
      std::cout << "faload";
      i += 1;
      break;
    }
    case 49: { // daload
      std::cout << "daload";
      i += 1;
      break;
    }
    case 50: { // aaload
      std::cout << "aaload";
      i += 1;
      break;
    }
    case 51: { // baload
      std::cout << "baload";
      i += 1;
      break;
    }
    case 52: { // caload
      std::cout << "caload";
      i += 1;
      break;
    }
    case 53: { // saload
      std::cout << "saload";
      i += 1;
      break;
    }
    case 54: { // istore
      u1 index = code.code[i + 1];
      std::cout << "istore " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 55: { // lstore
      u1 index = code.code[i + 1];
      std::cout << "lstore " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 56: { // fstore
      u1 index = code.code[i + 1];
      std::cout << "fstore " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 57: { // dstore
      u1 index = code.code[i + 1];
      std::cout << "dstore " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 58: { // astore
      u1 index = code.code[i + 1];
      std::cout << "astore " << static_cast<int>(index);
      i += 2;
      break;
    }
    case 59: { // istore_0
      std::cout << "istore_0";
      i += 1;
      break;
    }
    case 60: { // istore_1
      std::cout << "istore_1";
      i += 1;
      break;
    }
    case 62: { // istore_3
      std::cout << "istore_3";
      i += 1;
      break;
    }
    case 63: { // lstore_0
      std::cout << "lstore_0";
      i += 1;
      break;
    }
    case 64: { // lstore_1
      std::cout << "lstore_1";
      i += 1;
      break;
    }
    case 65: { // lstore_2
      std::cout << "lstore_2";
      i += 1;
      break;
    }
    case 66: { // lstore_3
      std::cout << "lstore_3";
      i += 1;
      break;
    }
    case 67: { // fstore_0
      std::cout << "fstore_0";
      i += 1;
      break;
    }
    case 68: { // fstore_1
      std::cout << "fstore_1";
      i += 1;
      break;
    }
    case 69: { // fstore_2
      std::cout << "fstore_2";
      i += 1;
      break;
    }
    case 70: { // fstore_3
      std::cout << "fstore_3";
      i += 1;
      break;
    }
    case 71: { // dstore_0
      std::cout << "dstore_0";
      i += 1;
      break;
    }
    case 72: { // dstore_1
      std::cout << "dstore_1";
      i += 1;
      break;
    }
    case 73: { // dstore_2
      std::cout << "dstore_2";
      i += 1;
      break;
    }
    case 74: { // dstore_3
      std::cout << "dstore_3";
      i += 1;
      break;
    }
    case 75: { // astore_0
      std::cout << "astore_0";
      i += 1;
      break;
    }
    case 79: { // iastore
      std::cout << "iastore";
      i += 1;
      break;
    }
    case 80: { // lastore
      std::cout << "lastore";
      i += 1;
      break;
    }
    case 81: { // fastore
      std::cout << "fastore";
      i += 1;
      break;
    }
    case 82: { // dastore
      std::cout << "dastore";
      i += 1;
      break;
    }
    case 84: { // bastore
      std::cout << "bastore";
      i += 1;
      break;
    }
    case 85: { // castore
      std::cout << "castore";
      i += 1;
      break;
    }
    case 86: { // sastore
      std::cout << "sastore";
      i += 1;
      break;
    }
    case 87: { // pop
      std::cout << "pop";
      i += 1;
      break;
    }
    case 88: { // pop2
      std::cout << "pop2";
      i += 1;
      break;
    }
    case 90: { // dup_x1
      std::cout << "dup_x1";
      i += 1;
      break;
    }
    case 91: { // dup_x2
      std::cout << "dup_x2";
      i += 1;
      break;
    }
    case 92: { // dup2
      std::cout << "dup2";
      i += 1;
      break;
    }
    case 93: { // dup2_x1
      std::cout << "dup2_x1";
      i += 1;
      break;
    }
    case 94: { // dup2_x2
      std::cout << "dup2_x2";
      i += 1;
      break;
    }
    case 95: { // swap
      std::cout << "swap";
      i += 1;
      break;
    }
    case 96: { // iadd
      std::cout << "iadd";
      i += 1;
      break;
    }
    case 97: { // ladd
      std::cout << "ladd";
      i += 1;
      break;
    }
    case 98: { // fadd
      std::cout << "fadd";
      i += 1;
      break;
    }
    case 99: { // dadd
      std::cout << "dadd";
      i += 1;
      break;
    }
    case 100: { // isub
      std::cout << "isub";
      i += 1;
      break;
    }
    case 101: { // lsub
      std::cout << "lsub";
      i += 1;
      break;
    }
    case 102: { // fsub
      std::cout << "fsub";
      i += 1;
      break;
    }
    case 103: { // dsub
      std::cout << "dsub";
      i += 1;
      break;
    }
    case 104: { // imul
      std::cout << "imul";
      i += 1;
      break;
    }
    case 105: { // lmul
      std::cout << "lmul";
      i += 1;
      break;
    }
    case 106: { // fmul
      std::cout << "fmul";
      i += 1;
      break;
    }
    case 107: { // dmul
      std::cout << "dmul";
      i += 1;
      break;
    }
    case 108: { // idiv
      std::cout << "idiv";
      i += 1;
      break;
    }
    case 109: { // ldiv
      std::cout << "ldiv";
      i += 1;
      break;
    }
    case 110: { // fdiv
      std::cout << "fdiv";
      i += 1;
      break;
    }
    case 111: { // ddiv
      std::cout << "ddiv";
      i += 1;
      break;
    }
    case 112: { // irem
      std::cout << "irem";
      i += 1;
      break;
    }
    case 113: { // lrem
      std::cout << "lrem";
      i += 1;
      break;
    }
    case 114: { // frem
      std::cout << "frem";
      i += 1;
      break;
    }
    case 115: { // drem
      std::cout << "drem";
      i += 1;
      break;
    }
    case 116: { // ineg
      std::cout << "ineg";
      i += 1;
      break;
    }
    case 117: { // lneg
      std::cout << "lneg";
      i += 1;
      break;
    }
    case 118: { // fneg
      std::cout << "fneg";
      i += 1;
      break;
    }
    case 119: { // dneg
      std::cout << "dneg";
      i += 1;
      break;
    }
    case 120: { // ishl
      std::cout << "ishl";
      i += 1;
      break;
    }
    case 121: { // lshl
      std::cout << "lshl";
      i += 1;
      break;
    }
    case 122: { // ishr
      std::cout << "ishr";
      i += 1;
      break;
    }
    case 123: { // lshr
      std::cout << "lshr";
      i += 1;
      break;
    }
    case 124: { // iushr
      std::cout << "iushr";
      i += 1;
      break;
    }
    case 125: { // lushr
      std::cout << "lushr";
      i += 1;
      break;
    }
    case 126: { // iand
      std::cout << "iand";
      i += 1;
      break;
    }
    case 127: { // land
      std::cout << "land";
      i += 1;
      break;
    }
    case 128: { // ior
      std::cout << "ior";
      i += 1;
      break;
    }
    case 129: { // lor
      std::cout << "lor";
      i += 1;
      break;
    }
    case 130: { // ixor
      std::cout << "ixor";
      i += 1;
      break;
    }
    case 131: { // lxor
      std::cout << "lxor";
      i += 1;
      break;
    }
    case 133: { // i2l
      std::cout << "i2l";
      i += 1;
      break;
    }
    case 134: { // i2f
      std::cout << "i2f";
      i += 1;
      break;
    }
    case 135: { // i2d
      std::cout << "i2d";
      i += 1;
      break;
    }
    case 136: { // l2i
      std::cout << "l2i";
      i += 1;
      break;
    }
    case 137: { // l2f
      std::cout << "l2f";
      i += 1;
      break;
    }
    case 138: { // l2d
      std::cout << "l2d";
      i += 1;
      break;
    }
    case 139: { // f2i
      std::cout << "f2i";
      i += 1;
      break;
    }
    case 140: { // f2l
      std::cout << "f2l";
      i += 1;
      break;
    }
    case 141: { // f2d
      std::cout << "f2d";
      i += 1;
      break;
    }
    case 142: { // d2i
      std::cout << "d2i";
      i += 1;
      break;
    }
    case 143: { // d2l
      std::cout << "d2l";
      i += 1;
      break;
    }
    case 144: { // d2f
      std::cout << "d2f";
      i += 1;
      break;
    }
    case 145: { // i2b
      std::cout << "i2b";
      i += 1;
      break;
    }
    case 146: { // i2c
      std::cout << "i2c";
      i += 1;
      break;
    }
    case 147: { // i2s
      std::cout << "i2s";
      i += 1;
      break;
    }
    case 148: { // lcmp
      std::cout << "lcmp";
      i += 1;
      break;
    }
    case 149: { // fcmpl
      std::cout << "fcmpl";
      i += 1;
      break;
    }
    case 150: { // fcmpg
      std::cout << "fcmpg";
      i += 1;
      break;
    }
    case 151: { // dcmpl
      std::cout << "dcmpl";
      i += 1;
      break;
    }
    case 152: { // dcmpg
      std::cout << "dcmpg";
      i += 1;
      break;
    }
    case 153: { // ifeq
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifeq " << (int16_t)offset;
      i += 3;
      break;
    }
    case 154: { // ifne
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifne " << (int16_t)offset;
      i += 3;
      break;
    }
    case 155: { // iflt
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "iflt " << (int16_t)offset;
      i += 3;
      break;
    }
    case 156: { // ifge
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifge " << (int16_t)offset;
      i += 3;
      break;
    }
    case 157: { // ifgt
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifgt " << (int16_t)offset;
      i += 3;
      break;
    }
    case 158: { // ifle
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifle " << (int16_t)offset;
      i += 3;
      break;
    }
    case 159: { // if_icmpeq
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_icmpeq " << (int16_t)offset;
      i += 3;
      break;
    }
    case 160: { // if_icmpne
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_icmpne " << (int16_t)offset;
      i += 3;
      break;
    }
    case 161: { // if_icmplt
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_icmplt " << (int16_t)offset;
      i += 3;
      break;
    }
    case 163: { // if_icmpgt
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_icmpgt " << (int16_t)offset;
      i += 3;
      break;
    }
    case 164: { // if_icmple
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_icmple " << (int16_t)offset;
      i += 3;
      break;
    }
    case 165: { // if_acmpeq
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_acmpeq " << (int16_t)offset;
      i += 3;
      break;
    }
    case 166: { // if_acmpne
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_acmpne " << (int16_t)offset;
      i += 3;
      break;
    }
    case 168: { // jsr
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "jsr " << (int16_t)offset;
      i += 3;
      break;
    }
    case 169: { // ret
      u1 index = code.code[i + 1];
      std::cout << "ret " << static_cast<int>(index);
      i += 2;
      break;
    }

    // --- SWITCHES (Opcodes 170-171) ---
    case 170: { // tableswitch
      std::cout << "tableswitch" << std::endl;
      u4 start_address = i + 1;
      u4 padding = (4 - (start_address % 4)) % 4;
      u4 read_idx = start_address + padding;

      u4 default_offset = (static_cast<u4>(code.code[read_idx]) << 24) |
                          (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                          (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                          static_cast<u4>(code.code[read_idx + 3]);
      read_idx += 4;

      u4 low = (static_cast<u4>(code.code[read_idx]) << 24) |
               (static_cast<u4>(code.code[read_idx + 1]) << 16) |
               (static_cast<u4>(code.code[read_idx + 2]) << 8) |
               static_cast<u4>(code.code[read_idx + 3]);
      read_idx += 4;

      u4 high = (static_cast<u4>(code.code[read_idx]) << 24) |
                (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                static_cast<u4>(code.code[read_idx + 3]);
      read_idx += 4;

      std::cout << "\t\t      default: " << static_cast<int32_t>(default_offset)
                << ", low: " << static_cast<int32_t>(low)
                << ", high: " << static_cast<int32_t>(high) << std::endl;

      u4 num_offsets = high - low + 1;
      for (u4 j = 0; j < num_offsets; j++) {
        u4 offset = (static_cast<u4>(code.code[read_idx]) << 24) |
                    (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                    (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                    static_cast<u4>(code.code[read_idx + 3]);
        read_idx += 4;
        std::cout << "\t\t      " << static_cast<int32_t>(low + j) << ": "
                  << static_cast<int32_t>(offset) << std::endl;
      }
      u4 total_bytes_read = read_idx - i;
      i += total_bytes_read;
      break;
    }
    case 171: { // lookupswitch
      std::cout << "lookupswitch" << std::endl;
      u4 start_address = i + 1;
      u4 padding = (4 - (start_address % 4)) % 4;
      u4 read_idx = start_address + padding;

      u4 default_offset = (static_cast<u4>(code.code[read_idx]) << 24) |
                          (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                          (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                          static_cast<u4>(code.code[read_idx + 3]);
      read_idx += 4;

      u4 npairs = (static_cast<u4>(code.code[read_idx]) << 24) |
                  (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                  (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                  static_cast<u4>(code.code[read_idx + 3]);
      read_idx += 4;

      std::cout << "\t\t      default: " << static_cast<int32_t>(default_offset)
                << ", npairs: " << npairs << std::endl;

      for (u4 j = 0; j < npairs; j++) {
        u4 match = (static_cast<u4>(code.code[read_idx]) << 24) |
                   (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                   (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                   static_cast<u4>(code.code[read_idx + 3]);
        read_idx += 4;

        u4 offset = (static_cast<u4>(code.code[read_idx]) << 24) |
                    (static_cast<u4>(code.code[read_idx + 1]) << 16) |
                    (static_cast<u4>(code.code[read_idx + 2]) << 8) |
                    static_cast<u4>(code.code[read_idx + 3]);
        read_idx += 4;

        std::cout << "\t\t      " << static_cast<int32_t>(match) << ": "
                  << static_cast<int32_t>(offset) << std::endl;
      }
      u4 total_bytes_read = read_idx - i;
      i += total_bytes_read;
      break;
    }
    case 172: { // ireturn
      std::cout << "ireturn";
      i += 1;
      break;
    }
    case 173: { // lreturn
      std::cout << "lreturn";
      i += 1;
      break;
    }
    case 174: { // freturn
      std::cout << "freturn";
      i += 1;
      break;
    }
    case 175: { // dreturn
      std::cout << "dreturn";
      i += 1;
      break;
    }
    case 176: { // areturn
      std::cout << "areturn";
      i += 1;
      break;
    }
    case 184: { // invokestatic
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "invokestatic #" << index;
      i += 3;
      break;
    }
    case 186: { // invokedynamic
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "invokedynamic #" << index;
      i += 5;
      break;
    }
    case 188: { // newarray
      u1 atype = code.code[i + 1];
      std::cout << "newarray (tipo " << static_cast<int>(atype) << ")";
      i += 2;
      break;
    }
    case 190: { // arraylength
      std::cout << "arraylength";
      i += 1;
      break;
    }
    case 191: { // athrow
      std::cout << "athrow";
      i += 1;
      break;
    }
    case 192: { // checkcast
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "checkcast #" << index;
      i += 3;
      break;
    }
    case 193: { // instanceof
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "instanceof #" << index;
      i += 3;
      break;
    }
    case 194: { // monitorenter
      std::cout << "monitorenter";
      i += 1;
      break;
    }
    case 195: { // monitorexit
      std::cout << "monitorexit";
      i += 1;
      break;
    }
    case 196: { // wide
      std::cout << "wide ";
      u1 opcode_wide = code.code[i + 1];
      if (opcode_wide == 132) { // iinc
        u2 index_local = (code.code[i + 2] << 8) | code.code[i + 3];
        u2 constante = (code.code[i + 4] << 8) | code.code[i + 5];
        std::cout << "iinc (var #" << index_local << " by "
                  << static_cast<int16_t>(constante) << ")";
        i += 6; // opcode + wide_opcode + 2*u2
      } else {
        // iload, lload, fload, dload, aload,
        // istore, lstore, fstore, dstore, astore, ret
        u2 index = (code.code[i + 2] << 8) | code.code[i + 3];
        std::cout << "(opcode " << static_cast<int>(opcode_wide) << ") #"
                  << index;
        i += 4; // opcode + wide_opcode + u2
      }
      break;
    }
    case 197: { // multianewarray
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      u1 dimensions = code.code[i + 3];
      std::cout << "multianewarray #" << index << " dim "
                << static_cast<int>(dimensions);
      i += 4;
      break;
    }
    case 198: { // ifnull
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifnull " << static_cast<int16_t>(offset);
      i += 3;
      break;
    }
    case 199: { // ifnonnull
      u2 offset = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ifnonnull " << static_cast<int16_t>(offset);
      i += 3;
      break;
    }
    case 200: { // goto_w
      u4 offset = (static_cast<u4>(code.code[i + 1]) << 24) |
                  (static_cast<u4>(code.code[i + 2]) << 16) |
                  (static_cast<u4>(code.code[i + 3]) << 8) |
                  static_cast<u4>(code.code[i + 4]);
      std::cout << "goto_w " << static_cast<int32_t>(offset);
      i += 5;
      break;
    }
    case 201: { // jsr_w
      u4 offset = (static_cast<u4>(code.code[i + 1]) << 24) |
                  (static_cast<u4>(code.code[i + 2]) << 16) |
                  (static_cast<u4>(code.code[i + 3]) << 8) |
                  static_cast<u4>(code.code[i + 4]);
      std::cout << "jsr_w " << static_cast<int32_t>(offset);
      i += 5;
      break;
    }
    // --- RESERVADOS (Opcodes 202, 254) ---
    case 202: { // breakpoint
      std::cout << "breakpoint (reservado)";
      i += 1;
      break;
    }
    case 254: { // impdep1
      std::cout << "impdep1 (reservado)";
      i += 1;
      break;
    }

    case 20: { // ldc2_w
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "ldc2_w #" << index;
      i += 3;
      break;
    }

    case 44: { // aload_2
      std::cout << "aload_2";
      i += 1;
      break;
    }

    case 45: { // aload_3
      std::cout << "aload_3";
      i += 1;
      break;
    }

    case 77: { // astore_2
      std::cout << "astore_2";
      i += 1;
      break;
    }

    case 78: { // astore_3
      std::cout << "astore_3";
      i += 1;
      break;
    }

    case 185: { // invokeinterface
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      u1 count = code.code[i + 3];
      std::cout << "invokeinterface #" << index << ", count " << (int)count;
      i += 5;
      break;
    }

    case 181: { // putfield
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "putfield #" << index;
      i += 3;
      break;
    }

    case 0: { // nop
      std::cout << "nop";
      i += 1;
      break;
    }

    case 2: { // iconst_m1
      std::cout << "iconst_m1";
      i += 1;
      break;
    }

    case 3: { // iconst_0
      std::cout << "iconst_0";
      i += 1;
      break;
    }

    case 16: { // bipush
      u1 byte = code.code[i + 1];
      std::cout << "bipush " << static_cast<int>(byte);
      i += 2;
      break;
    }

    case 13: { // lconst_0
      std::cout << "lconst_0";
      i += 1;
      break;
    }

    case 61: { // istore_2
      std::cout << "istore_2";
      i += 1;
      break;
    }

    case 28: { // iload_2
      std::cout << "iload_2";
      i += 1;
      break;
    }

    case 162: { // if_icmpge
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "if_icmpge #" << index;
      i += 3;
      break;
    }

    case 26: { // iload_0
      std::cout << "iload_0";
      i += 1;
      break;
    }

    case 180: { // getfield
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "getfield #" << index;
      i += 3;
      break;
    }

    case 4: { // iconst_1
      std::cout << "iconst_1";
      i += 1;
      break;
    }

    case 83: { // aastore
      std::cout << "aastore";
      i += 1;
      break;
    }

    case 132: { // iinc
      u1 index_local = code.code[i + 1];
      u1 constante = code.code[i + 2];
      std::cout << "iinc (var #" << static_cast<int>(index_local) << " by "
                << static_cast<int>(constante) << ")";
      i += 3;
      break;
    }

    case 1: { // aconst_null
      std::cout << "aconst_null";
      i += 1;
      break;
    }

    case 167: { // goto
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "goto #" << index;
      i += 3;
      break;
    }

    case 255: { // impdep2
      std::cout << "impdep2 (reservado)";
      i += 1;
      break;
    }

    case 189: { // anewarray
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "anewarray #" << index;
      i += 3;
      break;
    }

    case 42: { // aload_0
      std::cout << "aload_0";
      i += 1;
      break;
    }

    case 183: { // invokespecial
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "invokespecial #" << index;
      i += 3;
      break;
    }

    case 187: { // new
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "new #" << index;
      i += 3;
      break;
    }

    case 89: { // dup
      std::cout << "dup";
      i += 1;
      break;
    }

    case 76: { // astore_1
      std::cout << "astore_1";
      i += 1;
      break;
    }

    case 43: { // aload_1
      std::cout << "aload_1";
      i += 1;
      break;
    }

    case 178: { // getstatic
      u2 index = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "getstatic #" << index;
      i += 3;
      break;
    }

    case 18: { // ldc
      u1 index_ldc = code.code[i + 1];
      std::cout << "ldc #" << (int)index_ldc;
      i += 2;
      break;
    }

    case 182: { // invokevirtual
      u2 index_inv = (code.code[i + 1] << 8) | code.code[i + 2];
      std::cout << "invokevirtual #" << index_inv;
      i += 3;
      break;
    }

    case 177: { // return
      std::cout << "return";
      i += 1;
      break;
    }

    default: {
      std::cout << "opcode desconhecido: " << (int)opcode;
      i += 1;
      break;
    }
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;
  if (code.exception_table_length > 0) {
    std::cout << "\t\t  Exception table:" << std::endl;
    std::cout << "\t\t    from    to  target      type" << std::endl;

    for (const auto &ex : code.exception_table) {
      std::cout << "\t\t    " << std::setw(5) << ex.start_pc << " "
                << std::setw(5) << ex.end_pc << " " << std::setw(5)
                << ex.handler_pc << "      ";

      if (ex.catch_type == 0) {
        std::cout << "any (finally)" << std::endl;
      } else {
        // imprimir o nome da classe da exceção?? usar
        // get_utf8_from_pool(cf.constant_pool, ex.catch_type)
        std::cout << "Class #" << ex.catch_type << std::endl;
      }
    }
    std::cout << std::endl;
  }

  if (code.attributes_count > 0) {
    std::cout << "\t\t  Code Attributes (" << code.attributes_count
              << "):" << std::endl;
    print_read_attributes(code.attributes_count, code.attributes);
  }
}

// Helper para pegar nome de Classe (precisamos dele aqui)
static std::string
get_class_name_from_pool_viewer(const std::vector<ConstantPoolEntry> &pool,
                                u2 index) {
  try {
    const auto &class_entry = pool.at(index);
    if (class_entry.first != ConstantTag::CONSTANT_Class) {
      return "[ERRO: Nao e Class]";
    }
    u2 name_index = class_entry.second.class_info.name_index;
    return resolve_utf8(name_index, pool); // Reutiliza o helper
  } catch (const std::out_of_range &e) {
    return "[ERRO: Indice invalido]";
  }
}

void ClassFileViewer::print_read_attributes(
    u2 index, const std::vector<AttributeInfo> &entry) {
  for (u2 i = 0; i < index && i < entry.size(); i++) {
    const AttributeInfo &attribute = entry[i];

    std::cout << "\tAttribute name: \"" << attribute.attribute_name << "\""
              << " (index #" << attribute.attribute_name_index << ")"
              << std::endl;
    std::cout << "\tinfo length " << attribute.attribute_length << std::endl;

    if (attribute.attribute_name == "Code") {
      print_code_attribute(attribute.code_info);
    } else if (attribute.attribute_name == "ConstantValue") {
      u2 index = attribute.constantvalue_info.constantvalue_index;
      std::cout << "\t\tConstantValue: index #" << index << " ";

      if (index > 0 && index < cf.constant_pool.size()) {
        const auto &entry = cf.constant_pool[index];
        switch (entry.first) {
        case ConstantTag::CONSTANT_Integer:
          std::cout << "(Integer: " << (int32_t)entry.second.integer_info.bytes
                    << ")";
          break;
        case ConstantTag::CONSTANT_Float:
          std::cout << "(Float: ...)";
          break;
        case ConstantTag::CONSTANT_String: {
          u2 utf8_index = entry.second.string_info.string_index;
          std::cout << "(String: \""
                    << get_utf8_from_pool(cf.constant_pool, utf8_index)
                    << "\")";
        } break;
        default:
          std::cout << "(Tipo de pool desconhecido)";
        }
      }
      std::cout << std::endl;
    } else if (attribute.attribute_name == "Synthetic") {
      std::cout << "\t\tSynthetic: true" << std::endl;
    }

    else if (attribute.attribute_name == "Exceptions") {
      const auto &ex_info = attribute.exceptions_info;
      std::cout << "\t\tExceptions count: " << ex_info.number_of_exceptions
                << std::endl;
      for (u2 j = 0; j < ex_info.number_of_exceptions; j++) {
        u2 ex_index = ex_info.exception_index_table[j];
        std::string class_name =
            get_class_name_from_pool_viewer(cf.constant_pool, ex_index);
        std::cout << "\t\t  Exception #" << j << ": Class #" << ex_index
                  << " (\"" << class_name << "\")" << std::endl;
      }
    } else if (attribute.attribute_name == "InnerClasses") {
      const auto &ic_info = attribute.innerclasses_info;
      std::cout << "\t\tInnerClasses count: " << ic_info.number_of_classes
                << std::endl;
      for (u2 j = 0; j < ic_info.number_of_classes; j++) {
        const auto &inner_class = ic_info.classes[j];
        std::string inner_name =
            (inner_class.inner_name_index > 0)
                ? get_utf8_from_pool(cf.constant_pool,
                                     inner_class.inner_name_index)
                : "null";

        std::cout << "\t\t  InnerClass #" << j << ":" << std::endl;
        std::cout << "\t\t    inner_class_info:  #"
                  << inner_class.inner_class_info_index << std::endl;
        std::cout << "\t\t    outer_class_info:  #"
                  << inner_class.outer_class_info_index << std::endl;
        std::cout << "\t\t    inner_name:        #"
                  << inner_class.inner_name_index << " (\"" << inner_name
                  << "\")" << std::endl;
        std::cout << "\t\t    access_flags:      0x" << std::hex
                  << inner_class.inner_class_access_flags << std::dec
                  << std::endl;
      }
    }

    else {
      print_attribute_info_entry(attribute.attribute_length,
                                 attribute.unknown_info.info);
    }
  }
}

void ClassFileViewer::print_attribute_info_entry(u4 index,
                                                 const std::vector<u1> entry) {
  for (u4 i = 0; i < index && i < entry.size(); i++) {
    const u1 &info = entry[i];
    std::cout << "\t\t#" << std::setw(3) << i << " ";
    std::cout << "Info of attribute\t\t info = " << static_cast<int>(info)
              << " " << std::endl;
  }
}

std::string
ClassFileViewer::get_utf8_from_pool(const std::vector<ConstantPoolEntry> &pool,
                                    u2 index) {
  if (index > 0 && index < pool.size()) {
    const auto &entry = pool[index];
    if (entry.first == ConstantTag::CONSTANT_Utf8) {
      const auto &utf8_info = entry.second.utf8_info;
      return std::string(reinterpret_cast<const char *>(utf8_info.bytes),
                         utf8_info.length);
    }
  }
  return "<invalid index>";
}

void ClassFileViewer::print_methods_count(u2 count) {
  std::cout << "Methods count: " << count << std::endl;
}

void ClassFileViewer::print_methods() {
  std::cout << "\n--- Methods (" << cf.methods_count << ") ---" << std::endl;
  for (u2 i = 0; i < cf.methods_count; i++) {
    const auto &method = cf.methods[i];

    std::string name = get_utf8_from_pool(cf.constant_pool, method.name_index);
    std::string descriptor =
        get_utf8_from_pool(cf.constant_pool, method.descriptor_index);

    std::cout << "Method #" << i << ": " << name << descriptor << std::endl;
    std::cout << "  Name Index:      #" << method.name_index << std::endl;
    std::cout << "  Descriptor Index: #" << method.descriptor_index
              << std::endl;

    std::cout << "  Access Flags:    0x" << std::hex << method.access_flags
              << std::dec << " [";

    std::vector<std::string> flags;
    if (method.access_flags & 0x0001)
      flags.push_back("ACC_PUBLIC");
    if (method.access_flags & 0x0002)
      flags.push_back("ACC_PRIVATE");
    if (method.access_flags & 0x0004)
      flags.push_back("ACC_PROTECTED");
    if (method.access_flags & 0x0008)
      flags.push_back("ACC_STATIC");
    if (method.access_flags & 0x0010)
      flags.push_back("ACC_FINAL");
    // O jvm_types.h define ACC_Synthetic_Method = 0x1000
    if (method.access_flags & 0x1000)
      flags.push_back("ACC_SYNTHETIC");
    if (method.access_flags & 0x0040)
      flags.push_back("ACC_BRIDGE");
    if (method.access_flags & 0x0400)
      flags.push_back("ACC_ABSTRACT");

    for (u2 j = 0; j < flags.size(); j++) {
      std::cout << flags[j];
      if (j < flags.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "  Attributes Count: " << method.attributes_count << std::endl;

    if (method.attributes_count > 0) {
      print_read_attributes(method.attributes_count, method.attributes);
    }
  }
}
