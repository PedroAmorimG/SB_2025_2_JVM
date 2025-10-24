#include "class_parser.h"
#include "class_viewer.h" 
#include "jvm_types.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  bool debug = false;
  std::string filepath = "";

  for (int i = argc - 1; i >= 0; i--) {
    std::string arg(argv[i]);

    if (arg == "--debug") {
      debug = true;
    } else if (arg.rfind("filepath=", 0) == 0) {
      filepath = arg.substr(9);
    }
  }

  if (filepath == "") {
    std::cout << "File not selected. Use param 'filepath=' "
                 "to describe the .class file path"
              << std::endl;
    return 1;
  }

  std::ifstream file(filepath, std::ios::binary);

  if (!file) {
    std::cout << "Couldn't open the file" << std::endl;
    return 1;
  }

  ClassFile class_file;

  class_file.magic = read_magic(file, debug);
  if (class_file.magic != 0xCAFEBABE) {
    std::cout << "Not a valid .class file" << std::endl;
    return 1;
  }

  class_file.minor_version = read_minor_version(file, debug);
  class_file.major_version = read_major_version(file, debug);
  
  class_file.constant_pool_count = read_constant_pool_count(file, debug);
  class_file.constant_pool = read_constant_pool(file, class_file.constant_pool_count, debug);
  
  class_file.access_flags = read_access_flags(file, debug);
  class_file.this_class = read_this_class(file, debug);
  class_file.super_class = read_super_class(file, debug);
  
  class_file.interfaces_count = read_interface_count(file, debug);
  class_file.interfaces = read_interfaces(file, class_file.interfaces_count, debug);
  
  class_file.fields_count = read_field_count(file, debug);
  class_file.fields = read_fields(file, class_file.fields_count, class_file.constant_pool, debug);

  class_file.methods_count = read_methods_count(file, debug);
  class_file.methods = read_methods(file, class_file.methods_count, class_file.constant_pool, debug);
  
  class_file.attributes_count = read_attribute_count(file, debug);
  class_file.attributes = read_attributes(file, class_file.attributes_count, class_file.constant_pool, debug);

  file.close();

  if (debug) {
      print_methods(class_file);
      
      if (class_file.fields_count > 0) {
        std::cout << "\n--- Fields (" << class_file.fields_count << ") ---" << std::endl;
        for (u2 i = 0; i < class_file.fields_count; i++) {
            std::cout << "Field #" << i << ":" << std::endl; 
            print_read_fields(i, class_file.fields[i], class_file);
        }
      }
      
      if (class_file.attributes_count > 0) {
        std::cout << "\n--- Class Attributes (" << class_file.attributes_count << ") ---" << std::endl;
        print_read_attributes(class_file.attributes_count, class_file.attributes, class_file);      }
  }

  return 0;
}