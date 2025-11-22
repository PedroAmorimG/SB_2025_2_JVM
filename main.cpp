#include "./classfile/class_parser.h"
#include "./classfile/class_viewer.h"
#include "./classfile/classfile_types.h"
#include "./runtime/runtime_class_types.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

void printHelp(const std::string &progName) {
  std::cout << "Usage:\n"
            << "  " << progName << " [options]\n\n"
            << "Options:\n"
            << "  -f, --filepath <path>   Path to the .class file\n"
            << "  -i, --interactive       Execute the JVM (run main) instead "
               "of just showing\n"
            << "  -h, --help              Show this help message\n\n"
            << "Examples:\n"
            << "  " << progName << " -f Test.class\n"
            << "  " << progName << " -f Test.class -i\n";
}

int main(int argc, char *argv[]) {

  bool execMode = false; // changed: "interactive" → "execution mode"
  std::string filepath = "";
  std::string progName = argv[0];

  // Parse CLI args
  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);

    if (arg == "--help" || arg == "-h") {
      printHelp(progName);
      return 0;

    } else if (arg == "--interactive" || arg == "-i") {
      execMode = true;

    } else if (arg == "--filepath" || arg == "-f") {
      if (i + 1 < argc) {
        filepath = argv[++i];
      }

    } else if (arg.rfind("--filepath=", 0) == 0) {
      filepath = arg.substr(11);
    }
  }

  if (filepath.empty()) {
    std::cout << "Error: No file selected.\n\n";
    printHelp(progName);
    return 1;
  }

  try {
    if (!execMode) {
      ClassParser parser(filepath);
      ClassFile cf = parser.parse();
      ClassFileViewer viewer(cf);
      viewer.show_class_file();
    } else {
      Runtime rt;
      rt.start(filepath);

      std::cout << "Execution finished.\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
