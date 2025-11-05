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

  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);

    if (arg == "--debug" || arg == "-d") {
      debug = true;
    } else if (arg == "--filepath" || arg == "-f") {
      if (i + 1 < argc) {
        filepath = argv[++i];
      }
    } else if (arg.rfind("--filepath=", 0) == 0) {
      filepath = arg.substr(11);
    }
  }

  if (filepath.empty()) {
    std::cout << "File not selected.\n"
              << "Usage:\n " << argv[0] << " "
              << "  program.exe --filepath <path> [--debug]\n"
              << "Example:\n"
              << "  program.exe -f ./java/exemplos/vetor_8.class -d\n";
    return 1;
  }

  ClassParser parser(filepath);
  ClassFile cf = parser.parse();

  ClassFileViewer viewer(cf);
  viewer.show_class_file();

  return 0;
}
