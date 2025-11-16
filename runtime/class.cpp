#pragma once

#include "../classfile/class_parser.h"
#include "./runtime_class_types.h"

#include <string>
#include <unordered_map>
#include <vector>

u4 RuntimeClass::data_size() {
  u4 bytes = 0;
  for (auto field : fields)
    bytes += field.second.size_in_bytes();

  return bytes;
}

RuntimeMethod *RuntimeClass::find_method(const std::string &name,
                                         const std::string &descriptor) {
  for (auto method : methods) {
    if (method.second.name == name && method.second.descriptor == descriptor)
      return &method.second;
  }
  return nullptr;
}

RuntimeField *RuntimeClass::find_field(const std::string &name,
                                       const std::string &descriptor) {
  for (auto field : fields) {
    if (field.second.name == name && field.second.descriptor == descriptor)
      return &field.second;
  }
  return nullptr;
}

std::unique_ptr<RuntimeClass>
BootstrapClassLoader::load_class(const std::string &name) {
  ClassParser parser(name);
  std::unique_ptr<ClassFile> cf = std::make_unique<ClassFile>(parser.parse());

  std::unique_ptr<RuntimeClass> klass = build_runtime_class(std::move(cf));

  runtime->thread->call_stack.push_back(
      new Frame(klass.get()->find_method("<clinit>", "()V"), klass.get()));

  runtime->method_area->storeClass(std::move(klass));

  auto super_class = runtime->method_area->getClassRef(klass.get()->super_name);

  if (super_class == nullptr) { // Verifica se super_class já foi carregada
    load_class(klass.get()->super_name);
  }

  klass.get()->super_class = super_class;

  return klass;
}

std::unique_ptr<RuntimeClass>
BootstrapClassLoader::build_runtime_class(std::unique_ptr<ClassFile> cf) {
  std::string name = cf->resolve_utf8(cf->this_class);
  std::string super_name = cf->resolve_utf8(cf->super_class);
  u2 access_flags = cf->access_flags;
  RuntimeClass *super_class = nullptr;

  std::unordered_map<std::string, RuntimeField> fields;
  std::unordered_map<std::string, RuntimeMethod> methods;

  u4 offset_acc = 0;
  for (const auto &f : cf->fields) {
    std::string name = cf->resolve_utf8(f.name_index);
    std::string desc = cf->resolve_utf8(f.descriptor_index);

    std::string key = desc + " " + name;

    RuntimeField rf;

    rf.name = name;
    rf.descriptor = desc;
    rf.access_flags = f.access_flags;
    rf.is_static = (f.access_flags & ACC_Static_Field) != 0;

    rf.offset = offset_acc;
    offset_acc += rf.size_in_bytes();

    fields.emplace(key, rf);
  }

  for (const auto &m : cf->methods) {
    std::string name = cf->resolve_utf8(m.name_index);
    std::string desc = cf->resolve_utf8(m.descriptor_index);

    std::string key = desc + " " + name;

    RuntimeMethod rm;

    rm.name = name;
    rm.descriptor = desc;
    rm.access_flags = m.access_flags;
    rm.code = m.find_code_attribute();

    methods.emplace(key, rm);
  }

  std::unique_ptr<RuntimeClass> klass = std::make_unique<RuntimeClass>(
      name, super_name, access_flags, super_class, cf, fields, methods);

  return klass;
}