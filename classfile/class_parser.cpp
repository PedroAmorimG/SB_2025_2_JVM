#include "class_parser.h"
#include "class_viewer.h" // mantém debug existente
#include <algorithm>
#include <iostream>

static const std::string RUNTIME_CLASSPATH = ".\\runtime\\";

ClassParser::ClassParser(const std::string &filepath) {

  std::string sys_filepath(filepath);

  if (sys_filepath.find("java/") == 0) {
    if (sys_filepath.size() >= 6 &&
        sys_filepath.compare(sys_filepath.size() - 6, 6, ".class") == 0) {
      sys_filepath.erase(sys_filepath.size() - 6, 6);
    }
    sys_filepath = RUNTIME_CLASSPATH + sys_filepath + ".class";
  }

  file.open(sys_filepath, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open .class file: " + sys_filepath);
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

  if (magic != 0xCAFEBABE)
    throw std::runtime_error("Invalid .class file");

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
    f.attributes = readAttributes(f.attributes_count);
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
    m.attributes = readAttributes(m.attributes_count);
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
    a.attribute_name =
        getUtf8(a.attribute_name_index); // ✅ agora temos o nome do atributo

    if (a.attribute_name == "Code") {
      // ----------------------------
      // Code attribute
      // ----------------------------
      a.code_info.max_stack = read_u2();
      a.code_info.max_locals = read_u2();
      a.code_info.code_length = read_u4();

      a.code_info.code.resize(a.code_info.code_length);
      file.read(reinterpret_cast<char *>(a.code_info.code.data()),
                a.code_info.code_length);

      // exception table
      a.code_info.exception_table_length = read_u2();
      for (u2 j = 0; j < a.code_info.exception_table_length; j++) {
        ExceptionTableEntry e{};
        e.start_pc = read_u2();
        e.end_pc = read_u2();
        e.handler_pc = read_u2();
        e.catch_type = read_u2();
        a.code_info.exception_table.push_back(e);
      }

      a.code_info.attributes_count = read_u2();
      a.code_info.attributes = readAttributes(a.code_info.attributes_count);
    }

    else if (a.attribute_name == "ConstantValue") {
      a.constantvalue_info.constantvalue_index = read_u2();
    }

    else if (a.attribute_name == "Exceptions") {
      a.exceptions_info.number_of_exceptions = read_u2();
      for (u2 j = 0; j < a.exceptions_info.number_of_exceptions; j++) {
        u2 idx = read_u2();
        a.exceptions_info.exception_index_table.push_back(idx);
      }
    } else if (a.attribute_name == "LineNumberTable") {
      a.linenumbertable_info.line_number_table_length = read_u2();
      for (u2 j = 0; j < a.linenumbertable_info.line_number_table_length; j++) {
        LineNumberTableEntry entry{};
        entry.start_pc = read_u2();
        entry.line_number = read_u2();
        a.linenumbertable_info.line_number_table.push_back(entry);
      }
    }

    else if (a.attribute_name == "Synthetic") {
      // Não possui conteúdo
    } else if (a.attribute_name == "SourceFile") {
      a.sourcefile_info.sourcefile_index = read_u2();
    } else if (a.attribute_name == "InnerClasses") {
      a.innerclasses_info.number_of_classes = read_u2();
      for (u2 j = 0; j < a.innerclasses_info.number_of_classes; j++) {
        InnerClassInfo ic{};
        ic.inner_class_info_index = read_u2();
        ic.outer_class_info_index = read_u2();
        ic.inner_name_index = read_u2();
        ic.inner_class_access_flags = read_u2();
        a.innerclasses_info.classes.push_back(ic);
      }
    } else if (a.attribute_name == "StackMapTable") {
      a.stackmaptable_info.number_of_entries = read_u2();
      a.stackmaptable_info.entries.reserve(
          a.stackmaptable_info.number_of_entries);
      for (u2 i = 0; i < a.stackmaptable_info.number_of_entries; ++i) {
        a.stackmaptable_info.entries.push_back(read_stack_map_frame());
      }
    } else if (a.attribute_name == "LocalVariableTable") {
      a.localvariabletable_info.local_variable_table_length = read_u2();
      auto &vec = a.localvariabletable_info.local_variable_table;
      vec.reserve(a.localvariabletable_info.local_variable_table_length);

      for (u2 j = 0; j < a.localvariabletable_info.local_variable_table_length;
           j++) {
        LocalVariableTableEntry e{};
        e.start_pc = read_u2();
        e.length = read_u2();
        e.name_index = read_u2();
        e.descriptor_index = read_u2();
        e.index = read_u2();
        vec.push_back(e);
      }
    } else {
      // ----------------------------
      // Atributo desconhecido → fallback
      // ----------------------------
      a.unknown_info.info.resize(a.attribute_length);
      file.read(reinterpret_cast<char *>(a.unknown_info.info.data()),
                a.attribute_length);
    }

    attributes.push_back(a);
  }

  return attributes;
}

std::string ClassParser::getUtf8(u2 index) {
  if (index == 0 || index >= classfile.constant_pool.size())
    return "";

  const auto &entry = classfile.constant_pool[index];
  if (entry.first != ConstantTag::CONSTANT_Utf8)
    return "";

  const ConstantUTF8Info &utf = entry.second.utf8_info;
  return std::string(reinterpret_cast<const char *>(utf.bytes), utf.length);
}

VerificationTypeInfo ClassParser::read_verification_type_info() {
  VerificationTypeInfo vti{};
  u1 tag = read_u1();
  vti.tag = static_cast<VTTag>(tag);

  switch (vti.tag) {
  case VTTag::Object:
    vti.cpool_index = read_u2();
    break;
  case VTTag::Uninitialized:
    vti.offset = read_u2();
    break;
  default:
    break;
  }
  return vti;
}

StackMapFrame ClassParser::read_stack_map_frame() {
  StackMapFrame f{};
  u1 ft = read_u1();
  f.frame_type = ft;

  if (ft <= 63) {
    f.kind = SMFKind::Same;
    f.offset_delta = ft;
  } else if (ft <= 127) {
    f.kind = SMFKind::SameLocals1StackItem;
    f.offset_delta = ft - 64;
    f.stack_item = read_verification_type_info();
  } else if (ft == 247) {
    f.kind = SMFKind::SameLocals1StackItemExt;
    f.offset_delta = read_u2();
    f.stack_item = read_verification_type_info();
  } else if (ft >= 248 && ft <= 250) {
    f.kind = SMFKind::Chop;
    f.offset_delta = read_u2();
  } else if (ft == 251) {
    f.kind = SMFKind::SameExt;
    f.offset_delta = read_u2();
  } else if (ft >= 252 && ft <= 254) {
    f.kind = SMFKind::Append;
    f.offset_delta = read_u2();
    u1 k = ft - 251;
    f.locals_appended.reserve(k);
    for (u1 i = 0; i < k; ++i) {
      f.locals_appended.push_back(read_verification_type_info());
    }
  } else if (ft == 255) {
    f.kind = SMFKind::Full;
    f.offset_delta = read_u2();

    u2 number_of_locals = read_u2();
    f.locals_full.reserve(number_of_locals);
    for (u2 i = 0; i < number_of_locals; ++i)
      f.locals_full.push_back(read_verification_type_info());

    u2 number_of_stack_items = read_u2();
    f.stack_full.reserve(number_of_stack_items);
    for (u2 i = 0; i < number_of_stack_items; ++i)
      f.stack_full.push_back(read_verification_type_info());
  } else {

    f.kind = SMFKind::Same;
  }

  return f;
}

// ----------------------
// Método principal: parse()
// ----------------------

ClassFile ClassParser::parse() {

  classfile.magic = readMagic();
  classfile.minor_version = readMinorVersion();
  classfile.major_version = readMajorVersion();
  classfile.constant_pool_count = readConstantPoolCount();
  classfile.constant_pool = readConstantPool(classfile.constant_pool_count);
  classfile.access_flags = readAccessFlags();
  classfile.this_class = readThisClass();
  classfile.super_class = readSuperClass();
  classfile.interfaces_count = readInterfacesCount();
  classfile.interfaces = readInterfaces(classfile.interfaces_count);
  classfile.fields_count = readFieldsCount();
  classfile.fields = readFields(classfile.fields_count);
  classfile.methods_count = readMethodsCount();
  classfile.methods = readMethods(classfile.methods_count);
  classfile.attributes_count = readAttributesCount();
  classfile.attributes = readAttributes(classfile.attributes_count);

  return classfile;
}
