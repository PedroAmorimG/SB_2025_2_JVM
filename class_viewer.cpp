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

void print_read_fields(u2 index, const FieldInfo entry) {

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
                        entry.attributes); // Chamar no method também
}

void print_attribute_count(u2 index) {
  std::cout << "Attribute count index: #" << index << std::endl;
}

void print_read_attributes(u2 index, const std::vector<AttributeInfo> entry) {
  for (u2 i = 0; i < index && i < entry.size(); i++) {
    const AttributeInfo &attribute = entry[i];
    std::cout << "\tAttribute name_index " << attribute.attribute_name_index
              << " " << std::endl;
    std::cout << "\tinfo length " << attribute.attribute_length << std::endl;
    print_attribute_info_entry(attribute.attribute_length, attribute.info);
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
