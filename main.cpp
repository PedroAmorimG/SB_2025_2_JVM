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
    }
    else if (arg.rfind("filepath=", 0) == 0){
      filepath = arg.substr(9);
    }
  }

  if (filepath == "") {
    std::cout << "Arquivo não selecionado. Utilize o parâmetro 'filepath=' "
                 "para passar um caminho"
              << std::endl;
  }

  std::ifstream file(filepath);

  if (!file) {
    std::cout << "Não foi possível abrir o arquivo selecionado" << std::endl;
  }

  read_magic(file, debug);
  read_major_version(file, debug);
  read_minor_version(file, debug);
  u2 pool_count = read_constant_pool_count(file, debug);
  read_constant_pool(file, pool_count, debug);
  read_access_flags(file, debug);
  read_this_class(file, debug);
  read_super_class(file, debug);

  file.close();

  return 0;
}