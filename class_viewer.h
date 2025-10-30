#ifndef CLASS_VIEWER_H
#define CLASS_VIEWER_H

#include "jvm_types.h"
#include <string>
#include <vector>

void print_magic(u4 magic);
void print_major_version(u2 major);
void print_java_version(u2 major);
void print_minor_version(u2 minor);
void print_constant_pool_count(u2 count);
void print_constant_entry(u2 index, const std::vector<ConstantPoolEntry> &pool);
void print_access_flags(u2 flag);
void print_this_class(u2 index, const std::vector<ConstantPoolEntry> &pool);
void print_super_class(u2 index, const std::vector<ConstantPoolEntry> &pool);
void print_interface_count(u2 index);
void print_read_interfaces(u2 index, const u2 entry);
void print_field_count(u2 index);
void print_read_fields(u2 index, const FieldInfo entry, const ClassFile &cf);
void print_attribute_count(u2 index);
void print_read_attributes(u2 index, const std::vector<AttributeInfo> &entry,
                           const ClassFile &cf);
void print_attribute_info_entry(u4 index, const std::vector<u1> entry);
void print_code_attribute(const CodeAttribute &code, const ClassFile &cf);
// void print_methods_count(u2 count);

struct ClassFile;
void print_methods_count(u2 count);
void print_methods(const ClassFile &cf);

// para conseguir imprimir o nome do atributo da constant pool
std::string get_utf8_from_pool(const std::vector<ConstantPoolEntry> &pool,
                               u2 index);

#endif