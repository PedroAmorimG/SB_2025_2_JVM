#include "class_parser.h"
#include "class_viewer.h"
#include "jvm_types.h"
#include <utility>
#include <vector>
#include <iomanip>
#include <iostream>


u1 read_1byte(std::ifstream &file) {
  u1 byte;
  file.read(reinterpret_cast<char *>(&byte), 1);

  return byte;
}

u2 read_2bytes(std::ifstream &file) {
  u1 byte1 = read_1byte(file);
  u1 byte2 = read_1byte(file);

  return (byte1 << 8) | byte2;
}

u4 read_4bytes(std::ifstream &file) {
  u2 half_word1 = read_2bytes(file);
  u2 half_word2 = read_2bytes(file);

  return (half_word1 << 16) | half_word2;
}

u4 read_magic(std::ifstream &file, bool debug = false) {
  u4 magic = read_4bytes(file);

  if (debug) {
    print_magic(magic);
  }
  return magic;
}

u2 read_major_version(std::ifstream &file, bool debug = false) {
  u2 major = read_2bytes(file);

  if (debug) {
    print_major_version(major);
  }

  return major;
}

u2 read_minor_version(std::ifstream &file, bool debug = false) {
  u2 minor = read_2bytes(file);

  if (debug) {
    print_minor_version(minor);
  }

  return minor;
}

u2 read_constant_pool_count(std::ifstream &file, bool debug = false) {
  u2 count = read_2bytes(file);

  if (debug) {
    print_constant_pool_count(count);
  }

  return count;
}

ConstantPoolEntry read_constant_pool_entry(std::ifstream &file) {
  ConstantTag tag = static_cast<ConstantTag>(read_1byte(file));
  ConstantInfo info{};

  switch (tag) {
  case ConstantTag::CONSTANT_Class: {

    u2 name_index = read_2bytes(file);

    info.class_info.name_index = name_index;

    break;
  }
  case ConstantTag::CONSTANT_Fieldref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    info.fieldref_info.class_index = class_index;
    info.fieldref_info.name_and_type_index = name_and_type_index;

    break;
  }
  case ConstantTag::CONSTANT_Methodref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    info.methodref_info.class_index = class_index;
    info.methodref_info.name_and_type_index = name_and_type_index;

    break;
  }
  case ConstantTag::CONSTANT_InterfaceMethodref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    info.interface_methodref_info.class_index = class_index;
    info.interface_methodref_info.name_and_type_index = name_and_type_index;

    break;
  }
  case ConstantTag::CONSTANT_NameAndType: {
    u2 name_index = read_2bytes(file);
    u2 descriptor_index = read_2bytes(file);

    info.name_and_type_info.name_index = name_index;
    info.name_and_type_info.descriptor_index = descriptor_index;

    break;
  }
  case ConstantTag::CONSTANT_Utf8: {
    u2 length = read_2bytes(file);
    u1 *bytes = new u1[length];

    file.read(reinterpret_cast<char *>(bytes), length);

    info.utf8_info.length = length;
    info.utf8_info.bytes = bytes;

    break;
  }
  case ConstantTag::CONSTANT_String: {
    u2 string_index = read_2bytes(file);

    info.string_info.string_index = string_index;

    break;
  }
  case ConstantTag::CONSTANT_Integer: {
    u4 bytes = read_4bytes(file);

    info.integer_info.bytes = bytes;

    break;
  }
  case ConstantTag::CONSTANT_Float: {
    u4 bytes = read_4bytes(file);

    info.float_info.bytes = bytes;

    break;
  }
  case ConstantTag::CONSTANT_Long: {
    u4 high_bytes = read_4bytes(file);
    u4 low_bytes = read_4bytes(file);

    info.long_info.high_bytes = high_bytes;
    info.long_info.low_bytes = low_bytes;

    break;
  }
  case ConstantTag::CONSTANT_Double: {
    u4 high_bytes = read_4bytes(file);
    u4 low_bytes = read_4bytes(file);

    info.double_info.high_bytes = high_bytes;
    info.double_info.low_bytes = low_bytes;

    break;
  }
  default: {
    info.empty = EmptyInfo{};
  }
  }
  return std::make_pair(tag, info);
}

std::vector<ConstantPoolEntry> read_constant_pool(std::ifstream &file, u2 count, bool debug = false) {
  std::vector<ConstantPoolEntry> pool;
  ConstantInfo empty_info;
  empty_info.empty = EmptyInfo{};
  pool.emplace_back(ConstantTag::None, empty_info);

  for (u2 i = 1; i < count; i++) {
    ConstantPoolEntry entry = read_constant_pool_entry(file);

    if (debug) {
      print_constant_entry(i, entry);
    }

    pool.push_back(entry);

    if (entry.first == ConstantTag::CONSTANT_Long ||
        entry.first == ConstantTag::CONSTANT_Double) {
      pool.push_back(ConstantPoolEntry(ConstantTag::None, empty_info));
      i++;
    }
  }

  return pool;
}

u2 read_access_flags(std::ifstream &file, bool debug = false) {
  u2 flag = read_2bytes(file);

  if (debug) {
    print_access_flags(flag);
  }

  return flag;
}

u2 read_this_class(std::ifstream &file, bool debug = false) {
  u2 index = read_2bytes(file);

  if (debug) {
    print_this_class(index);
  }

  return index;
}

u2 read_super_class(std::ifstream &file, bool debug = false) {
  u2 index = read_2bytes(file);

  if (debug) {
    print_super_class(index);
  }

  return index;
}

u2 read_interface_count(std::ifstream &file, bool debug) {
  u2 index = read_2bytes(file);

  if (debug) {
    print_interface_count(index);
  }

  return index;
}

std::vector<u2> read_interfaces(std::ifstream &file, u2 count, bool debug) {
  std::vector<u2> interfaces;

  for (u2 i = 0; i < count; i++) {
    u2 entry = read_2bytes(file);

    if (debug) {
      print_read_interfaces(i, entry);
    }

    interfaces.push_back(entry);
  }

  return interfaces;
}

u2 read_field_count(std::ifstream &file, bool debug) {
  u2 index = read_2bytes(file);

  if (debug) {
    print_field_count(index);
  }

  return index;
}

std::vector<FieldInfo> read_fields(std::ifstream &file, u2 count, std::vector<ConstantPoolEntry>& cp, bool debug) {
    std::vector<FieldInfo> fields_vector;
    
    for (u2 i = 0; i < count; i++) {
        FieldInfo entry{}; 

        entry.access_flags = static_cast<FieldAccessFlag>(read_2bytes(file));
        entry.name_index = read_2bytes(file);
        entry.descriptor_index = read_2bytes(file);
        entry.attributes_count = read_2bytes(file);
        entry.attributes = read_attributes(file, entry.attributes_count, cp, debug);

        fields_vector.push_back(entry);
    }
    return fields_vector;
}

std::vector<AttributeInfo> read_attributes(std::ifstream &file, u2 count, std::vector<ConstantPoolEntry>& cp, bool debug) {
    std::vector<AttributeInfo> attributes_vector;

    for (u2 i = 0; i < count; i++) {
        AttributeInfo entry{};

        entry.attribute_name_index = read_2bytes(file);
        entry.attribute_length = read_4bytes(file);
        //  Usar 'cp' para descobrir o nome do atributo 
        entry.attribute_name = get_utf8_from_pool(cp, entry.attribute_name_index);

        if (entry.attribute_name == "Code") {
            //não precisamos mais do info
            entry.code_info.max_stack = read_2bytes(file);
            entry.code_info.max_locals = read_2bytes(file);
            entry.code_info.code_length = read_4bytes(file);
            
            std::cout << "[DEBUG]   Lido code_length = " << entry.code_info.code_length << std::flush;
            entry.code_info.code.resize(entry.code_info.code_length);
            file.read(reinterpret_cast<char*>(entry.code_info.code.data()), entry.code_info.code_length);
            // PULAR A TABELA DE EXCEÇÕES POR ENQUANTO
            u2 exception_table_length = read_2bytes(file);
            //file.seekg(exception_table_length * 8, std::ios_base::cur); // 8 bytes por entrada
            //std::vector<u1> exception_table_dummy(exception_table_length * 8);
            //file.read(reinterpret_cast<char*>(exception_table_dummy.data()), exception_table_dummy.size());
            // Vamos ler e descartar os bytes da tabela de exceção, um por um.
            u4 exception_table_bytes = exception_table_length * 8;
            for (u4 j = 0; j < exception_table_bytes; j++) {
                read_1byte(file); // Lê 1 byte e o descarta
            }
            entry.code_info.attributes_count = read_2bytes(file);
            entry.code_info.attributes = read_attributes(file, entry.code_info.attributes_count, cp, debug);


        } 
        // continuar else ifs para sourcefile, constant value, etc...
        else {
            //atributo desconhecido 
            std::cout << "[DEBUG] Lendo atributo '" << entry.attribute_name << "' (" << entry.attribute_length << " bytes)." << std::flush;
            entry.unknown_info.info.resize(entry.attribute_length);
            file.read(reinterpret_cast<char*>(entry.unknown_info.info.data()), entry.attribute_length);
        }


        attributes_vector.push_back(entry);
    }

    return attributes_vector;
}

u2 read_attribute_count(std::ifstream &file, bool debug) {
  u2 count = read_2bytes(file);
  if (debug) {
    print_attribute_count(count);
  }
  return count;
}

u2 read_methods_count(std::ifstream &file, bool debug = false) {
    u2 count = read_2bytes(file);

    if (debug) {
        print_methods_count(count);
    }

    return count;
}

std::vector<MethodInfo> read_methods(std::ifstream &file, u2 count, std::vector<ConstantPoolEntry>& cp, bool debug) {
    std::vector<MethodInfo> methods;
    for (u2 i = 0; i < count; i++) {
        MethodInfo method_info{};

        method_info.access_flags = static_cast<MethodAccessFlag>(read_2bytes(file));
        method_info.name_index = read_2bytes(file);
        method_info.descriptor_index = read_2bytes(file);
        method_info.attributes_count = read_2bytes(file);
        
        method_info.attributes = read_attributes(file, method_info.attributes_count, cp, debug);

        methods.push_back(method_info);
    }
    return methods;
}
