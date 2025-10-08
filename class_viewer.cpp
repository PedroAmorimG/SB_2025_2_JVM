#include "class_viewer.h"
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

void print_constant_entry(size_t index, const ConstantPoolEntry &entry) {
  const ConstantTag tag = entry.first;
  const ConstantInfo &info = entry.second;

  std::cout << "#" << std::setw(3) << index << " ";

  switch (tag) {
  case CONSTANT_Class: {
    const auto &v = std::get<ConstantClassInfo>(info);
    std::cout << "Class\t\tname_index = " << v.name_index;
    break;
  }

  case CONSTANT_Fieldref: {
    const auto &v = std::get<ConstantFieldrefInfo>(info);
    std::cout << "Fieldref\tclass_index = " << v.class_index
              << ", name_and_type_index = " << v.name_and_type_index;
    break;
  }

  case CONSTANT_Methodref: {
    const auto &v = std::get<ConstantMethodrefInfo>(info);
    std::cout << "Methodref\tclass_index = " << v.class_index
              << ", name_and_type_index = " << v.name_and_type_index;
    break;
  }

  case CONSTANT_InterfaceMethodref: {
    const auto &v = std::get<ConstantInterfaceMethodrefInfo>(info);
    std::cout << "InterfaceMethodref\tclass_index = " << v.class_index
              << ", name_and_type_index = " << v.name_and_type_index;
    break;
  }

  case CONSTANT_String: {
    const auto &v = std::get<ConstantStringInfo>(info);
    std::cout << "String\t\tstring_index = " << v.string_index;
    break;
  }

  case CONSTANT_Integer: {
    const auto &v = std::get<ConstantIntegerInfo>(info);
    std::cout << "Integer\t\tbytes = 0x" << std::hex << v.bytes << std::dec;
    break;
  }

  case CONSTANT_Float: {
    const auto &v = std::get<ConstantFloatInfo>(info);
    float f;
    std::memcpy(&f, &v.bytes, sizeof(float));
    std::cout << "Float\t\tvalue = " << f << " (bits = 0x" << std::hex
              << v.bytes << std::dec << ")";
    break;
  }

  case CONSTANT_Long: {
    const auto &v = std::get<ConstantLongInfo>(info);
    uint64_t value = (static_cast<uint64_t>(v.high_bytes) << 32) | v.low_bytes;
    std::cout << "Long\t\tvalue = " << value;
    break;
  }

  case CONSTANT_Double: {
    const auto &v = std::get<ConstantDoubleInfo>(info);
    uint64_t bits = (static_cast<uint64_t>(v.high_bytes) << 32) | v.low_bytes;
    double d;
    std::memcpy(&d, &bits, sizeof(double));
    std::cout << "Double\t\tvalue = " << d;
    break;
  }

  case CONSTANT_NameAndType: {
    const auto &v = std::get<ConstantNameAndTypeInfo>(info);
    std::cout << "NameAndType\tname_index = " << v.name_index
              << ", descriptor_index = " << v.descriptor_index;
    break;
  }

  case CONSTANT_Utf8: {
    const auto &v = std::get<ConstantUTF8Info>(info);
    std::cout << "Utf8\t\t\"";
    for (auto c : v.bytes)
      std::cout << static_cast<char>(c);
    std::cout << "\" (" << v.length << " bytes)";
    break;
  }

  case None: {
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
