#include "class_parser.h"
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

  u4 magic = read_magic(file, debug);

  if (magic != 0xCAFEBABE) {
    std::cout << "Not a valid .class file" << std::endl;
    return 1;
  }

  read_major_version(file, debug);
  read_minor_version(file, debug);
  u2 pool_count = read_constant_pool_count(file, debug);
  read_constant_pool(file, pool_count, debug);
  read_access_flags(file, debug);
  read_this_class(file, debug);
  read_super_class(file, debug);
  u2 interfaces_count = read_interface_count(file, debug);
  read_interfaces(file, interfaces_count, debug);
  // u2 field_count = read_field_count(file, debug);
  // read_fields(file, field_count, debug);

  file.close();

  return 0;
}
