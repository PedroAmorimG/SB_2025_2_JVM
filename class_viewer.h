#ifndef CLASS_VIEWER_H
#define CLASS_VIEWER_H

#include "jvm_types.h"

void print_magic(u4 magic);
void print_major_version(u2 major);
void print_minor_version(u2 minor);
void print_constant_pool_count(u2 count);
void print_constant_entry(u2 index, const ConstantPoolEntry &entry);
void print_access_flags(u2 flag);
void print_this_class(u2 index);
void print_super_class(u2 index);
void print_interface_count(u2 index);
void print_read_interfaces(u2 index, const u2 entry);
void print_field_count(u2 index);
void print_read_fields(u2 index, const FieldInfo entry);
void print_attribute_count(u2 index);
void print_read_attributes(u2 index, const AttributeInfo entry);
void print_attribute_info_entry(u4 index, const u2 entry);

#endif