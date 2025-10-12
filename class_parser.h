#include "jvm_types.h"
#include <fstream>
#include <vector>

u1 read_1byte(std::ifstream &file);
u2 read_2bytes(std::ifstream &file);
u4 read_4bytes(std::ifstream &file);

u4 read_magic(std::ifstream &file, bool debug);
u2 read_major_version(std::ifstream &file, bool debug);
u2 read_minor_version(std::ifstream &file, bool debug);
u2 read_constant_pool_count(std::ifstream &file, bool debug);
std::vector<ConstantPoolEntry> read_constant_pool(std::ifstream &file, u2 count,
                                                  bool debug);
u2 read_access_flags(std::ifstream &file, bool debug);
u2 read_this_class(std::ifstream &file, bool debug);
u2 read_super_class(std::ifstream &file, bool debug);
u2 read_interface_count(std::ifstream &file, bool debug);
std::vector<u2> read_interfaces(std::ifstream &file, u2 count, bool debug);
u2 read_field_count(std::ifstream &file, bool debug);
std::vector<FieldInfo> read_fields(std::ifstream &file, u2 count, bool debug);
u2 read_method_count(std::ifstream &file, bool debug);
std::vector<MethodInfo> read_methods(std::ifstream &file, u2 count, bool debug);
u2 read_attribute_count(std::ifstream &file, bool debug);
std::vector<AttributeInfo> read_attributes(std::ifstream &file, u2 count, bool debug);
