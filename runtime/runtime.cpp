#include "./runtime_class_types.h"
#include <string>

void Runtime::start(std::string filepath) {
  class_loader->load_class(filepath);
}