#include "./runtime_class_types.h"

#include <string>
#include <utility>
#include <vector>

RuntimeClass *MethodArea::getClassRef(const std::string &name) {
  auto it = classes.find(name);
  if (it == classes.end())
    return nullptr;
  return it->second.get();
}

void MethodArea::storeClass(std::unique_ptr<RuntimeClass> klass) {
  classes.emplace(klass->name, std::move(klass));
}