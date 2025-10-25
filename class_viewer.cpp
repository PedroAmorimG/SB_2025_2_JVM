#include "class_viewer.h"
#include "jvm_types.h"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

void print_magic(u4 magic) {
  std::cout << std::hex << std::setfill('0');
  std::cout << "Magic: 0x" << std::setw(8) << magic << std::endl;
  std::cout << std::dec; // volta para decimal
}

void print_major_version(u2 major) {
  std::cout << "Major version: " << major << std::endl;
}

void print_minor_version(u2 minor) {
  std::cout << "Minor version: " << minor << std::endl;
}

void print_constant_pool_count(u2 count) {
  std::cout << "Constant pool count: " << count << std::endl;
}

void print_constant_entry(u2 index, const ConstantPoolEntry &entry) {
  const ConstantTag tag = entry.first;
  const ConstantInfo &info = entry.second;

  std::cout << "#" << std::setw(3) << index << " ";

  switch (tag) {
  case ConstantTag::CONSTANT_Class: {
    const auto &v = info.class_info;
    std::cout << "Class\t\tname_index = " << v.name_index;
    break;
  }

  case ConstantTag::CONSTANT_Fieldref: {
    const auto &v = info.fieldref_info;
    std::cout << "Fieldref\tclass_index = " << v.class_index
              << ", name_and_type_index = " << v.name_and_type_index;
    break;
  }

  case ConstantTag::CONSTANT_Methodref: {
    const auto &v = info.methodref_info;
    std::cout << "Methodref\tclass_index = " << v.class_index
              << ", name_and_type_index = " << v.name_and_type_index;
    break;
  }

  case ConstantTag::CONSTANT_InterfaceMethodref: {
    const auto &v = info.interface_methodref_info;
    std::cout << "InterfaceMethodref\tclass_index = " << v.class_index
              << ", name_and_type_index = " << v.name_and_type_index;
    break;
  }

  case ConstantTag::CONSTANT_String: {
    const auto &v = info.string_info;
    std::cout << "String\t\tstring_index = " << v.string_index;
    break;
  }

  case ConstantTag::CONSTANT_Integer: {
    const auto &v = info.integer_info;
    std::cout << "Integer\t\tbytes = 0x" << std::hex << v.bytes << std::dec;
    break;
  }

  case ConstantTag::CONSTANT_Float: {
    const auto &v = info.float_info;
    float f;
    std::memcpy(&f, &v.bytes, sizeof(float));
    std::cout << "Float\t\tvalue = " << f << " (bits = 0x" << std::hex
              << v.bytes << std::dec << ")";
    break;
  }

  case ConstantTag::CONSTANT_Long: {
    const auto &v = info.long_info;
    uint64_t value = (static_cast<uint64_t>(v.high_bytes) << 32) | v.low_bytes;
    std::cout << "Long\t\tvalue = " << value;
    break;
  }

  case ConstantTag::CONSTANT_Double: {
    const auto &v = info.double_info;
    uint64_t bits = (static_cast<uint64_t>(v.high_bytes) << 32) | v.low_bytes;
    double d;
    std::memcpy(&d, &bits, sizeof(double));
    std::cout << "Double\t\tvalue = " << d;
    break;
  }

  case ConstantTag::CONSTANT_NameAndType: {
    const auto &v = info.name_and_type_info;
    std::cout << "NameAndType\tname_index = " << v.name_index
              << ", descriptor_index = " << v.descriptor_index;
    break;
  }

  case ConstantTag::CONSTANT_Utf8: {
    const auto &v = info.utf8_info;
    std::cout << "Utf8\t\t\"";
    for (u2 i = 0; i < v.length; i++)
      std::cout << static_cast<char>(v.bytes[i]);
    std::cout << "\" (" << v.length << " bytes)";
    break;
  }

  case ConstantTag::None: {
    std::cout << "(empty)";
    break;
  }

  default:
    std::cout << "Tag desconhecido (" << static_cast<int>(tag) << ")";
    break;
  }

  std::cout << "\n";
}

void print_access_flags(u2 flag) {
  std::cout << "Access flags: 0x" << std::hex << flag << std::dec << " [";

  std::vector<std::string> flags;

  if (flag & 0x0001) {
    flags.push_back("ACC_PUBLIC");
  }
  if (flag & 0x0010) {
    flags.push_back("ACC_FINAL");
  }
  if (flag & 0x0020) {
    flags.push_back("ACC_SUPER");
  }
  if (flag & 0x0200) {
    flags.push_back("ACC_INTERFACE");
  }
  if (flag & 0x0400) {
    flags.push_back("ACC_ABSTRACT");
  }
  if (flag & 0x1000) {
    flags.push_back("ACC_SYNTHETIC");
  }
  if (flag & 0x2000) {
    flags.push_back("ACC_ANNOTATION");
  }
  if (flag & 0x4000) {
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

void print_this_class(u2 index) {
  std::cout << "This class index: #" << index << std::endl;
}

void print_super_class(u2 index) {
  std::cout << "Super class index: #" << index << std::endl;
}

void print_interface_count(u2 index) {
  std::cout << "Interface count index: #" << index << std::endl;
}

void print_read_interfaces(u2 index, const u2 entry) {
  std::cout << "#" << std::setw(3) << index << " ";
  std::cout << "Class\t\tname_index = " << entry << std::endl;
}

void print_field_count(u2 index) {
  std::cout << "Field count index: #" << index << std::endl;
}

void print_read_fields(u2 index, const FieldInfo entry, const ClassFile& cf) {

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
  print_read_attributes(entry.attributes_count,
                        entry.attributes, cf);
}

void print_attribute_count(u2 count) {
  std::cout << "Class Attributes count: " << count << std::endl;
}

void print_code_attribute(const CodeAttribute& code, const ClassFile& cf) {
    std::cout << "\t\tCode:" << std::endl;
    std::cout << "\t\t  stack=" << code.max_stack;
    std::cout << ", locals=" << code.max_locals << std::endl;
    
    std::cout << "\t\t  bytecode (" << code.code_length << " bytes):" << std::endl;

    for (u4 i = 0; i < code.code_length; ) {
    u1 opcode = code.code[i];
    std::cout << "\t\t    " << i << ": ";

    switch (opcode) {
        case 178: { // getstatic   
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
            std::cout << "getstatic #" << index;
            // (Aqui você usaria o cf.constant_pool para imprimir o que é o #index)
            i += 3; // Avança 3 bytes (opcode + 2 bytes de index)
            break;
        } 

        case 18: { // ldc  
            u1 index_ldc = code.code[i+1];
            std::cout << "ldc #" << (int)index_ldc;
            i += 2; // Avança 2 bytes (opcode + 1 byte de index)
            break;
        } 

        case 182: { // invokevirtual   
            u2 index_inv = (code.code[i+1] << 8) | code.code[i+2];
            std::cout << "invokevirtual #" << index_inv;
            i += 3;
            break;
        } 

        case 177: { // return  
            std::cout << "return";
            i += 1; // Avança 1 byte
            break;
        } 

        default: { 
            std::cout << "opcode desconhecido: " << (int)opcode;
            i += 1;
            break;
        } 
    }
// ...
    std::cout << std::endl;
}

    
    if (code.attributes_count > 0) {
        std::cout << "\t\t  Code Attributes (" << code.attributes_count << "):" << std::endl;
        print_read_attributes(code.attributes_count, code.attributes, cf);
    }
}

void print_read_attributes(u2 index, const std::vector<AttributeInfo>& entry, const ClassFile& cf) {
  for (u2 i = 0; i < index && i < entry.size(); i++) {
    const AttributeInfo &attribute = entry[i];

    std::cout << "\tAttribute name: \"" << attribute.attribute_name << "\"" 
              << " (index #" << attribute.attribute_name_index << ")" << std::endl;
    std::cout << "\tinfo length " << attribute.attribute_length << std::endl;

    if (attribute.attribute_name == "Code") {
        print_code_attribute(attribute.code_info, cf);
    } 
    else {
        print_attribute_info_entry(attribute.attribute_length, attribute.unknown_info.info);
    }
  }
}

void print_attribute_info_entry(u4 index, const std::vector<u1> entry) {
  for (u4 i = 0; i < index && i < entry.size(); i++) {
    const u1 &info = entry[i];
    std::cout << "\t\t#" << std::setw(3) << i << " ";
    std::cout << "Info of attribute\t\t info = " << static_cast<int>(info)
              << " " << std::endl;
  }
}

std::string get_utf8_from_pool(const std::vector<ConstantPoolEntry>& pool, u2 index) {
    if (index > 0 && index < pool.size()) {
        const auto& entry = pool[index];
        if (entry.first == ConstantTag::CONSTANT_Utf8) {
            const auto& utf8_info = entry.second.utf8_info;
            return std::string(reinterpret_cast<const char*>(utf8_info.bytes), utf8_info.length);
        }
    }
    return "<invalid index>";
}

void print_methods_count(u2 count) {
    std::cout << "Methods count: " << count << std::endl;
}

void print_methods(const ClassFile &cf) {
    std::cout << "\n--- Methods (" << cf.methods_count << ") ---" << std::endl;
    for (u2 i = 0; i < cf.methods_count; i++) {
        const auto& method = cf.methods[i];
        
        std::string name = get_utf8_from_pool(cf.constant_pool, method.name_index);
        std::string descriptor = get_utf8_from_pool(cf.constant_pool, method.descriptor_index);

        std::cout << "Method #" << i << ": " << name << descriptor << std::endl;
        std::cout << "  Name Index:      #" << method.name_index << std::endl;
        std::cout << "  Descriptor Index: #" << method.descriptor_index << std::endl;
        std::cout << "  Access Flags:    0x" << std::hex << method.access_flags << std::dec << std::endl;
        std::cout << "  Attributes Count: " << method.attributes_count << std::endl;

        if (method.attributes_count > 0) {
          print_read_attributes(method.attributes_count, method.attributes, cf);       
        }
    }
}