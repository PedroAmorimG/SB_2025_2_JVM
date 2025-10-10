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
  ConstantTag tag = static_cast<ConstantTag>(read_1byte(file));
  ConstantInfo info;

  switch (tag) {
  case ConstantTag::CONSTANT_Class: {

    u2 name_index = read_2bytes(file);

    info.class_info = ConstantClassInfo{.name_index = name_index};

    break;
  }
  case ConstantTag::CONSTANT_Fieldref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    info.fieldref_info = ConstantFieldrefInfo{.class_index = class_index, .name_and_type_index = name_and_type_index};

    break;
  }
  case ConstantTag::CONSTANT_Methodref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    info.methodref_info = ConstantMethodrefInfo{.class_index= class_index, .name_and_type_index= name_and_type_index};
    
    break;
  }
  case ConstantTag::CONSTANT_InterfaceMethodref: {
    u2 class_index = read_2bytes(file);
    u2 name_and_type_index = read_2bytes(file);

    info.interface_methodref_info = ConstantInterfaceMethodrefInfo{.class_index= class_index, .name_and_type_index= name_and_type_index};

    break;
  }
  case ConstantTag::CONSTANT_NameAndType: {
    u2 name_index = read_2bytes(file);
    u2 descriptor_index = read_2bytes(file);

    info.name_and_type_info = ConstantNameAndTypeInfo{.name_index=name_index, .descriptor_index=descriptor_index};

    break;

  }
  case ConstantTag::CONSTANT_Utf8: {
    u2 length = read_2bytes(file);
    u1 *bytes;

    file.read(reinterpret_cast<char *>(bytes), length);

    info.utf8_info = ConstantUTF8Info{.length = length, .bytes = bytes};

    break;
  }
  case ConstantTag::CONSTANT_String: {
    u2 string_index = read_2bytes(file);

    info.string_info = ConstantStringInfo{.string_index = string_index};

    break;
  }
  case ConstantTag::CONSTANT_Integer: {
    u4 bytes = read_4bytes(file);

    info.integer_info = ConstantIntegerInfo{.bytes = bytes};

    break;
  }
  case ConstantTag::CONSTANT_Float: {
    u4 bytes = read_4bytes(file);

    info.float_info = ConstantFloatInfo{.bytes = bytes};

    break;
  }
  case ConstantTag::CONSTANT_Long: {
    u4 high_bytes = read_4bytes(file);
    u4 low_bytes = read_4bytes(file);

    info.long_info = ConstantLongInfo{.high_bytes = high_bytes, .low_bytes= low_bytes};

    break;
  }
  case ConstantTag::CONSTANT_Double: {
    u4 high_bytes = read_4bytes(file);
    u4 low_bytes = read_4bytes(file);

    info.double_info = ConstantDoubleInfo{.high_bytes = high_bytes, .low_bytes = low_bytes};

    break;
  }
  default: {
    info.empty = EmptyInfo{};
  }
  return std::make_pair(tag, info);
  }
}

std::vector<ConstantPoolEntry> read_constant_pool(std::ifstream &file, u2 count,
                                                  bool debug = false) {
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
