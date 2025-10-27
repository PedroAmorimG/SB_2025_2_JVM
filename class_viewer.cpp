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
        case 5: { // iconst_2
            std::cout << "iconst_2";
            i += 1;
            break;
        }

        case 20: { // ldc2_w
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
            u1 count = code.code[i+3];
            std::cout << "invokeinterface #" << index << ", count " << (int)count;
            i += 5;
            break;
        }

        case 181: { // putfield
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u1 byte = code.code[i+1];
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
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u1 index_local = code.code[i+1];
            u1 constante = code.code[i+2];
            std::cout << "iinc (var #" << static_cast<int>(index_local) 
                      << " by " << static_cast<int>(constante) << ")";
            i += 3; 
            break;
        }

        case 1: { // aconst_null
            std::cout << "aconst_null"; 
            i += 1;
            break;
        }

        case 167: { // goto
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
            std::cout << "invokespecial #" << index;
            i += 3; 
            break;
        }

        case 187: { // new
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
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
            u2 index = (code.code[i+1] << 8) | code.code[i+2];
            std::cout << "getstatic #" << index;
            i += 3; 
            break;
        } 

        case 18: { // ldc  
            u1 index_ldc = code.code[i+1];
            std::cout << "ldc #" << (int)index_ldc;
            i += 2; 
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
        
        for (const auto& ex : code.exception_table) {
            std::cout << "\t\t    " 
                      << std::setw(5) << ex.start_pc << " " 
                      << std::setw(5) << ex.end_pc << " "
                      << std::setw(5) << ex.handler_pc << "      ";
            
            if (ex.catch_type == 0) {
                std::cout << "any (finally)" << std::endl;
            } else {
                // imprimir o nome da classe da exceção?? usar get_utf8_from_pool(cf.constant_pool, ex.catch_type)
                std::cout << "Class #" << ex.catch_type << std::endl;
            }
        }
        std::cout << std::endl;
    }

    if (code.attributes_count > 0) {
        std::cout << "\t\t  Code Attributes (" << code.attributes_count << "):" << std::endl;
        print_read_attributes(code.attributes_count, code.attributes, cf);
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
    return get_utf8_from_pool(pool, name_index); // Reutiliza o helper
  } catch (const std::out_of_range &e) {
    return "[ERRO: Indice invalido]";
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
    else if (attribute.attribute_name == "ConstantValue") {
        u2 index = attribute.constantvalue_info.constantvalue_index;
        std::cout << "\t\tConstantValue: index #" << index << " ";

        if (index > 0 && index < cf.constant_pool.size()) {
            const auto& entry = cf.constant_pool[index];
            switch (entry.first) {
                case ConstantTag::CONSTANT_Integer:
                    std::cout << "(Integer: " << (int32_t)entry.second.integer_info.bytes << ")";
                    break;
                case ConstantTag::CONSTANT_Float:
                    std::cout << "(Float: ...)"; 
                    break;
                case ConstantTag::CONSTANT_String:
                    {
                      u2 utf8_index = entry.second.string_info.string_index;
                      std::cout << "(String: \"" << get_utf8_from_pool(cf.constant_pool, utf8_index) << "\")";
                    }
                    break;
                default:
                    std::cout << "(Tipo de pool desconhecido)";
            }
        }
        std::cout << std::endl;
    }
    else if (attribute.attribute_name == "Synthetic") {
        std::cout << "\t\tSynthetic: true" << std::endl;
    }

    else if (attribute.attribute_name == "Exceptions") {
        const auto& ex_info = attribute.exceptions_info;
        std::cout << "\t\tExceptions count: " << ex_info.number_of_exceptions << std::endl;
        for (u2 j = 0; j < ex_info.number_of_exceptions; j++) {
            u2 ex_index = ex_info.exception_index_table[j];
            std::string class_name = get_class_name_from_pool_viewer(cf.constant_pool, ex_index);
            std::cout << "\t\t  Exception #" << j << ": Class #" << ex_index 
                      << " (\"" << class_name << "\")" << std::endl;
        }
    }
    else if (attribute.attribute_name == "InnerClasses") {
        const auto& ic_info = attribute.innerclasses_info;
        std::cout << "\t\tInnerClasses count: " << ic_info.number_of_classes << std::endl;
        for (u2 j = 0; j < ic_info.number_of_classes; j++) {
            const auto& inner_class = ic_info.classes[j];
            std::string inner_name = (inner_class.inner_name_index > 0)
                                     ? get_utf8_from_pool(cf.constant_pool, inner_class.inner_name_index)
                                     : "null";
            
            std::cout << "\t\t  InnerClass #" << j << ":" << std::endl;
            std::cout << "\t\t    inner_class_info:  #" << inner_class.inner_class_info_index << std::endl;
            std::cout << "\t\t    outer_class_info:  #" << inner_class.outer_class_info_index << std::endl;
            std::cout << "\t\t    inner_name:        #" << inner_class.inner_name_index << " (\"" << inner_name << "\")" << std::endl;
            std::cout << "\t\t    access_flags:      0x" << std::hex 
                      << inner_class.inner_class_access_flags << std::dec << std::endl;
        }
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

        std::cout << "  Access Flags:    0x" << std::hex << method.access_flags << std::dec << " [";
        
        std::vector<std::string> flags;
        if (method.access_flags & 0x0001) flags.push_back("ACC_PUBLIC");
        if (method.access_flags & 0x0002) flags.push_back("ACC_PRIVATE");
        if (method.access_flags & 0x0004) flags.push_back("ACC_PROTECTED");
        if (method.access_flags & 0x0008) flags.push_back("ACC_STATIC");
        if (method.access_flags & 0x0010) flags.push_back("ACC_FINAL");
        // O jvm_types.h define ACC_Synthetic_Method = 0x1000
        if (method.access_flags & 0x1000) flags.push_back("ACC_SYNTHETIC"); 
        if (method.access_flags & 0x0040) flags.push_back("ACC_BRIDGE");
        if (method.access_flags & 0x0400) flags.push_back("ACC_ABSTRACT");
        
        for (u2 j = 0; j < flags.size(); j++) {
            std::cout << flags[j];
            if (j < flags.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        std::cout << "  Attributes Count: " << method.attributes_count << std::endl;

        if (method.attributes_count > 0) {
          print_read_attributes(method.attributes_count, method.attributes, cf);       
        }
    }
}