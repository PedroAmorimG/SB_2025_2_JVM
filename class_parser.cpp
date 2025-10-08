#include "class_parser.h"
#include "class_viewer.h"
#include "jvm_types.h"
#include <utility>
#include <vector>

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
  ConstantTag tag{read_1byte(file)};

  switch (tag) {
  case CONSTANT_Class: {

    u2 name_index = read_2bytes(file);
    return std::pair(tag, ConstantClassInfo{.name_index = name_index});
  }
  case CONSTANT_Fieldref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    return std::pair(
        tag, ConstantFieldrefInfo{.class_index = class_index,
                                  .name_and_type_index = name_and_type_index});
  }
  case CONSTANT_Methodref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    return std::pair(
        tag, ConstantMethodrefInfo{.class_index = class_index,
                                   .name_and_type_index = name_and_type_index});
  }
  case CONSTANT_InterfaceMethodref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    return std::pair(tag, ConstantInterfaceMethodrefInfo{
                              .class_index = class_index,
                              .name_and_type_index = name_and_type_index});
  }
  case CONSTANT_NameAndType: {
    u2 name_index = read_2bytes(file);
    u2 descriptor_index = read_2bytes(file);

    return std::pair(
        tag, ConstantNameAndTypeInfo{.name_index = name_index,
                                     .descriptor_index = descriptor_index});
  }
  case CONSTANT_Utf8: {
    ConstantUTF8Info info;
    info.length = read_2bytes(file);
    info.bytes.resize(info.length);
    file.read(reinterpret_cast<char *>(info.bytes.data()), info.length);

    return std::pair(tag, info);
  }
  case CONSTANT_String: {
    u2 string_index = read_2bytes(file);

    return std::pair(tag, ConstantStringInfo{.string_index = string_index});
  }
  case CONSTANT_Integer: {
    u4 bytes = read_4bytes(file);

    return std::pair(tag, ConstantIntegerInfo{.bytes = bytes});
  }
  case CONSTANT_Float: {
    u4 bytes = read_4bytes(file);

    return std::pair(tag, ConstantFloatInfo{.bytes = bytes});
  }
  case CONSTANT_Long: {
    u4 high_bytes = read_4bytes(file);
    u4 low_bytes = read_4bytes(file);

    return std::pair(tag, ConstantLongInfo{.high_bytes = high_bytes,
                                           .low_bytes = low_bytes});
  }
  case CONSTANT_Double: {
    u4 high_bytes = read_4bytes(file);
    u4 low_bytes = read_4bytes(file);

    return std::pair(tag, ConstantDoubleInfo{.high_bytes = high_bytes,
                                             .low_bytes = low_bytes});
  }
  default: {
    return std::pair(ConstantTag::None, EmptyInfo{});
  }
  }
}

std::vector<ConstantPoolEntry> read_constant_pool(std::ifstream &file, u2 count,
                                                  bool debug = false) {
  std::vector<ConstantPoolEntry> pool;
  pool.emplace_back(ConstantTag::None, EmptyInfo{});

  for (u2 i = 1; i < count; i++) {
    ConstantPoolEntry entry = read_constant_pool_entry(file);

    if (debug) {
      print_constant_entry(i, entry);
    }

    pool.push_back(entry);
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
