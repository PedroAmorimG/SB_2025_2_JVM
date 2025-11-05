#ifndef CLASS_VIEWER_H
#define CLASS_VIEWER_H

#include "jvm_types.h"
#include <string>
#include <vector>

class ClassFileViewer {
public:
  explicit ClassFileViewer(ClassFile cf);
  void show_class_file();

private:
  ClassFile cf;

  void print_magic();
  void print_major_version();
  void print_java_version();
  void print_minor_version();
  void print_constant_pool_count();
  void print_constant_entry(u2 index);
  void print_constant_pool();
  void print_access_flags();
  void print_this_class();
  void print_super_class();
  void print_interface_count();
  void print_interfaces();
  void print_field_count();
  void print_fields();
  void print_attribute_count();
  void print_attributes(u2 index, const std::vector<AttributeInfo> &entry);
  void print_attribute_info_entry(u4 index, const std::vector<u1> entry);
  void print_code_attribute(const CodeAttribute &code);

  void print_methods_count();
  void print_methods();

  std::string get_utf8_from_pool(const std::vector<ConstantPoolEntry> &pool,
                                 u2 index);
};

#endif