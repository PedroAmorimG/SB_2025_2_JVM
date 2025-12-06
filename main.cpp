#include "./classfile/class_parser.h"
#include "./classfile/class_viewer.h"
#include "./classfile/classfile_types.h"
#include "./runtime/debug.h"
#include "./runtime/runtime_class_types.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void printHelp(const std::string &progName) {
  std::cout << "Usage:\n"
            << "  " << progName << " [options]\n\n"
            << "Options:\n"
            << "  -f, --filepath <path>   Path to the .class file\n"
            << "  -i, --interactive       Execute the JVM (run main) instead "
               "of just showing\n"
            << "  -d, --debug             Enable verbose JVM debug logs\n"
            << "  -a, --args <args...>    Arguments forwarded to "
               "Java main(String[])\n"
            << "  -h, --help              Show this help message\n\n"
            << "Examples:\n"
            << "  " << progName << " -f Test.class\n"
            << "  " << progName << " -f Test.class -i\n"
            << "  " << progName
            << " -f Test.class -i -a \"first second\" third\n";
}

int main(int argc, char *argv[]) {

  bool execMode = false; // changed: "interactive" → "execution mode"
  std::string filepath = "";
  std::string progName = argv[0];
  std::vector<std::string> mainArgs;

  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);

    if (arg == "--help" || arg == "-h") {
      printHelp(progName);
      return 0;

    } else if (arg == "--interactive" || arg == "-i") {
      execMode = true;

    } else if (arg == "--debug" || arg == "-d") {
      g_debug_enabled = true;

    } else if (arg == "--args" || arg == "-a") {
      for (++i; i < argc; ++i) {
        std::string candidate(argv[i]);
        if (!candidate.empty() && candidate[0] == '-') {
          --i;
          break;
        }
        mainArgs.push_back(candidate);
      }

    } else if (arg.rfind("--args=", 0) == 0) {
      std::string list = arg.substr(7);

      std::string acc;

      for (auto c : list) {
        if (c != ' ') {
          acc += c;
        } else {
          mainArgs.push_back(acc);
          acc = "";
        }
      }

      if (!acc.empty())
        mainArgs.push_back(acc);

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
      rt.start(filepath, mainArgs);

      std::cout << "Execution finished.\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
