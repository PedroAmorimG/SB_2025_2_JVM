#include "class_parser.h"
#include "class_viewer.h" // mantém debug existente
#include <algorithm>
#include <iostream>

ClassParser::ClassParser(const std::string &filepath) {
  file.open(filepath, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open .class file: " + filepath);
  }
}

// ----------------------
// Métodos utilitários de leitura
// ----------------------

u1 ClassParser::read_u1() {
  u1 byte;
  file.read(reinterpret_cast<char *>(&byte), 1);
  return byte;
}

u2 ClassParser::read_u2() {
  u1 byte1 = read_u1();
  u1 byte2 = read_u1();
  return (byte1 << 8) | byte2;
}

u4 ClassParser::read_u4() {
  u2 half1 = read_u2();
  u2 half2 = read_u2();
  return (half1 << 16) | half2;
}

// ----------------------
// Leitura dos campos principais do ClassFile
// ----------------------

u4 ClassParser::readMagic() {
  u4 magic = read_u4();

  return magic;
}

u2 ClassParser::readMinorVersion() {
  u2 minor = read_u2();

  return minor;
}

u2 ClassParser::readMajorVersion() {
  u2 major = read_u2();

  return major;
}

u2 ClassParser::readConstantPoolCount() {
  u2 count = read_u2();

  return count;
}

// ----------------------
// Constant Pool
// ----------------------

ConstantPoolEntry ClassParser::readConstantPoolEntry() {
  ConstantTag tag = static_cast<ConstantTag>(read_u1());
  ConstantInfo info{};

  switch (tag) {
  case ConstantTag::CONSTANT_Class:
    info.class_info.name_index = read_u2();
    break;
  case ConstantTag::CONSTANT_Fieldref:
    info.fieldref_info.class_index = read_u2();
    info.fieldref_info.name_and_type_index = read_u2();
    break;
  case ConstantTag::CONSTANT_Methodref:
    info.methodref_info.class_index = read_u2();
    info.methodref_info.name_and_type_index = read_u2();
    break;
  case ConstantTag::CONSTANT_InterfaceMethodref:
    info.interface_methodref_info.class_index = read_u2();
    info.interface_methodref_info.name_and_type_index = read_u2();
    break;
  case ConstantTag::CONSTANT_NameAndType:
    info.name_and_type_info.name_index = read_u2();
    info.name_and_type_info.descriptor_index = read_u2();
    break;
  case ConstantTag::CONSTANT_Utf8: {
    u2 length = read_u2();
    u1 *bytes = new u1[length];
    file.read(reinterpret_cast<char *>(bytes), length);

    info.utf8_info.length = length;
    info.utf8_info.bytes = bytes;
    break;
  }
  case ConstantTag::CONSTANT_String:
    info.string_info.string_index = read_u2();
    break;
  case ConstantTag::CONSTANT_Integer:
    info.integer_info.bytes = read_u4();
    break;
  case ConstantTag::CONSTANT_Float:
    info.float_info.bytes = read_u4();
    break;
  case ConstantTag::CONSTANT_Long:
    info.long_info.high_bytes = read_u4();
    info.long_info.low_bytes = read_u4();
    break;
  case ConstantTag::CONSTANT_Double:
    info.double_info.high_bytes = read_u4();
    info.double_info.low_bytes = read_u4();
    break;
  default:
    info.empty = EmptyInfo{};
  }

  return std::make_pair(tag, info);
}

std::vector<ConstantPoolEntry> ClassParser::readConstantPool(u2 count) {
  std::vector<ConstantPoolEntry> pool;

  ConstantInfo empty_info;
  empty_info.empty = EmptyInfo{};
  pool.emplace_back(ConstantTag::None, empty_info);

  for (u2 i = 1; i < count; i++) {
    ConstantPoolEntry entry = readConstantPoolEntry();
    pool.push_back(entry);

    if (entry.first == ConstantTag::CONSTANT_Long ||
        entry.first == ConstantTag::CONSTANT_Double) {
      pool.push_back(ConstantPoolEntry(ConstantTag::None, empty_info));
      i++;
    }
  }

  return pool;
}

// ----------------------
// Outros componentes da estrutura
// ----------------------

u2 ClassParser::readAccessFlags() {
  u2 flags = read_u2();

  return flags;
}

u2 ClassParser::readThisClass() {
  u2 index = read_u2();
  // debug da classe depende do CP, resolveremos na ClassFile
  return index;
}

u2 ClassParser::readSuperClass() {
  u2 index = read_u2();
  return index;
}

u2 ClassParser::readInterfacesCount() {
  u2 count = read_u2();

  return count;
}

std::vector<u2> ClassParser::readInterfaces(u2 count) {
  std::vector<u2> interfaces;
  for (u2 i = 0; i < count; i++) {
    u2 index = read_u2();
    interfaces.push_back(index);
  }
  return interfaces;
}

u2 ClassParser::readFieldsCount() {
  u2 count = read_u2();

  return count;
}

std::vector<FieldInfo> ClassParser::readFields(u2 count) {
  std::vector<FieldInfo> fields;
  for (u2 i = 0; i < count; i++) {
    FieldInfo f{};
    f.access_flags = static_cast<FieldAccessFlag>(read_u2());
    f.name_index = read_u2();
    f.descriptor_index = read_u2();
    f.attributes_count = read_u2();
    fields.push_back(f);
  }
  return fields;
}

u2 ClassParser::readMethodsCount() {
  u2 count = read_u2();

  return count;
}

std::vector<MethodInfo> ClassParser::readMethods(u2 count) {
  std::vector<MethodInfo> methods;
  for (u2 i = 0; i < count; i++) {
    MethodInfo m{};
    m.access_flags = static_cast<MethodAccessFlag>(read_u2());
    m.name_index = read_u2();
    m.descriptor_index = read_u2();
    m.attributes_count = read_u2();
    m.attributes = {}; // Resolveremos depois
    methods.push_back(m);
  }
  return methods;
}

u2 ClassParser::readAttributesCount() {
  u2 count = read_u2();

  return count;
}

std::vector<AttributeInfo> ClassParser::readAttributes(u2 count) {
  std::vector<AttributeInfo> attributes;
  for (u2 i = 0; i < count; i++) {
    AttributeInfo a{};
    a.attribute_name_index = read_u2();
    a.attribute_length = read_u4();

    // Copiando como estava → resolver depois!
    a.unknown_info.info.resize(a.attribute_length);
    file.read(reinterpret_cast<char *>(a.unknown_info.info.data()),
              a.attribute_length);

    attributes.push_back(a);
  }
  return attributes;
}

// ----------------------
// Método principal: parse()
// ----------------------

ClassFile ClassParser::parse() {
  ClassFile cf{};

  cf.magic = readMagic();
  cf.minor_version = readMinorVersion();
  cf.major_version = readMajorVersion();
  cf.constant_pool_count = readConstantPoolCount();
  cf.constant_pool = readConstantPool(cf.constant_pool_count);
  cf.access_flags = readAccessFlags();
  cf.this_class = readThisClass();
  cf.super_class = readSuperClass();
  cf.interfaces_count = readInterfacesCount();
  cf.interfaces = readInterfaces(cf.interfaces_count);
  cf.fields_count = readFieldsCount();
  cf.fields = readFields(cf.fields_count);
  cf.methods_count = readMethodsCount();
  cf.methods = readMethods(cf.methods_count);
  cf.attributes_count = readAttributesCount();
  cf.attributes = readAttributes(cf.attributes_count);

  return cf;
}
