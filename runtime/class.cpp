#include "../classfile/class_parser.h"
#include "./runtime_class_types.h"

#include <iostream>
#include <memory>
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
  for (auto &method : methods) {
    if (method.second.name == name && method.second.descriptor == descriptor)
      return &method.second;
  }
  return nullptr;
}

RuntimeField *RuntimeClass::find_field(const std::string &name,
                                       const std::string &descriptor) {
  for (auto &field : fields) {
    if (field.second.name == name && field.second.descriptor == descriptor)
      return &field.second;
  }
  return nullptr;
}

RuntimeClass *BootstrapClassLoader::load_class(const std::string &name) {
  ClassParser parser(name);
  std::unique_ptr<ClassFile> cf(new ClassFile(parser.parse()));

  std::unique_ptr<RuntimeClass> klass = build_runtime_class(std::move(cf));
  auto klass_ptr = klass.get();

  runtime->method_area->storeClass(std::move(klass));

  if (auto class_init = klass_ptr->find_method("<clinit>", "()V")) {
    auto frame = new Frame(class_init, klass_ptr);
    frame->init(class_init->code->max_locals, class_init->code->max_stack);
    runtime->thread->call_stack.push_back(frame);
  }

  RuntimeClass *super_ref = nullptr;

  if (!klass_ptr->super_name.empty()) {
    if (auto loaded_class =
            runtime->method_area->getClassRef(klass_ptr->super_name)) {
      super_ref = loaded_class;
    } else {
      super_ref = load_class(klass_ptr->super_name);
    }
  }

  klass_ptr->super_class = super_ref;

  std::cout << "JVM runtime log | " << "Class loaded: " << klass_ptr->name
            << "\n";
  return klass_ptr;
}

std::unique_ptr<RuntimeClass>
BootstrapClassLoader::build_runtime_class(std::unique_ptr<ClassFile> cf) {
  std::string name = cf->resolve_utf8(cf->this_class);
  std::unordered_map<std::string, RuntimeField> fields;
  std::unordered_map<std::string, RuntimeMethod> methods;

  auto compute_arg_slots = [](const std::string &desc, bool is_static) -> int {
    int slots = is_static ? 0 : 1; // this
    size_t i = desc.find('(');
    if (i == std::string::npos)
      return slots;
    ++i;
    while (i < desc.size() && desc[i] != ')') {
      char c = desc[i];
      if (c == 'J' || c == 'D') {
        slots += 2;
        ++i;
      } else if (c == 'L') {
        slots += 1;
        while (i < desc.size() && desc[i] != ';')
          ++i;
        if (i < desc.size() && desc[i] == ';')
          ++i;
      } else if (c == '[') {
        slots += 1;
        ++i;
        while (i < desc.size() && desc[i] == '[')
          ++i;
        if (i < desc.size() && desc[i] == 'L') {
          while (i < desc.size() && desc[i] != ';')
            ++i;
          if (i < desc.size() && desc[i] == ';')
            ++i;
        } else {
          ++i;
        }
      } else {
        slots += 1;
        ++i;
      }
    }
    return slots;
  };

  u4 offset_acc = 0;
  u4 static_offset_acc = 0;
  for (const auto &f : cf->fields) {
    std::string name = cf->resolve_utf8(f.name_index);
    std::string desc = cf->resolve_utf8(f.descriptor_index);

    std::string key = desc + " " + name;

    RuntimeField rf;

    rf.name = name;
    rf.descriptor = desc;
    rf.access_flags = f.access_flags;
    rf.is_static = (f.access_flags & ACC_Static_Field) != 0;
    rf.is_64bit = (!desc.empty() && (desc[0] == 'J' || desc[0] == 'D'));
    rf.owner = nullptr;

    if (rf.is_static) {
      rf.static_offset = static_offset_acc;
      static_offset_acc += rf.size_in_bytes();
      rf.offset = 0;
    } else {
      rf.offset = offset_acc;
      offset_acc += rf.size_in_bytes();
    }

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
    rm.owner = nullptr;
    bool is_static = (m.access_flags & ACC_Static_Method) != 0;
    rm.arg_slots = compute_arg_slots(desc, is_static);

    methods.emplace(key, rm);
  }

  std::unique_ptr<RuntimeClass> klass(new RuntimeClass());
  klass->name = name;
  if (cf->super_class != 0) {
    klass->super_name = cf->resolve_utf8(cf->super_class);
  }
  klass->access_flags = cf->access_flags;
  klass->class_file = std::move(cf);
  klass->fields = std::move(fields);
  klass->methods = std::move(methods);
  klass->super_class = nullptr;
  klass->static_data.resize(static_offset_acc);

  // fix owner ptrs
  for (auto &fkv : klass->fields) {
    fkv.second.owner = klass.get();
  }
  for (auto &mkv : klass->methods) {
    mkv.second.owner = klass.get();
  }

  return klass;
}

Thread::Thread(Runtime *rt) : runtime(rt) {
  interpreter = new Interpreter(this);
}
Thread::~Thread() {
  delete interpreter;
  for (auto frame : call_stack)
    delete frame;
};
Runtime::~Runtime() {
  delete thread;
  delete method_area;
  delete class_loader;
}
