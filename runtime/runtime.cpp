#include "./array.h"
#include "./runtime_class_types.h"
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

class JavaException : public std::exception {
public:
  explicit JavaException(RuntimeObject *obj,
                         std::string msg = "Java exception thrown")
      : obj_(obj), msg_(std::move(msg)) {}

  const char *what() const noexcept override { return msg_.c_str(); }
  RuntimeObject *object() const noexcept { return obj_; }

private:
  RuntimeObject *obj_;
  std::string msg_;
};

void throw_java_exception(RuntimeObject *exception_obj) {
  throw JavaException(exception_obj, "Java athrow");
}

namespace {
RuntimeArray *build_args_array(Runtime *runtime,
                               const std::vector<std::string> &args) {
  if (args.empty())
    return nullptr;

  RuntimeClass *string_class = nullptr;
  try {
    string_class = runtime->method_area->getClassRef("java/lang/String");
    if (!string_class) {
      string_class = runtime->class_loader->load_class("java/lang/String");
    }
  } catch (const std::exception &e) {
    std::cerr << "Warning: could not materialize args for main: " << e.what()
              << std::endl;
    return nullptr;
  }

  RuntimeArray *array =
      RuntimeArray::create_reference(string_class, args.size());

  RuntimeField *value_field = string_class->find_field("value", "[B");

  for (size_t idx = 0; idx < args.size(); ++idx) {
    RuntimeObject *str_obj = new RuntimeObject(string_class);

    if (value_field) {
      const std::string &arg = args[idx];
      RuntimeArray *bytes =
          RuntimeArray::create_primitive(nullptr, arg.size(), sizeof(u1));
      for (size_t j = 0; j < arg.size(); ++j) {
        bytes->write_primitive<u1>(j, static_cast<u1>(arg[j]));
      }

      str_obj->write_field<u4>(
          *value_field, static_cast<u4>(reinterpret_cast<uintptr_t>(bytes)));
    }

    array->write_ref(idx, str_obj);
  }

  return array;
}
} // namespace

void Runtime::start(std::string filepath,
                    const std::vector<std::string> &args) {
  if (auto *boot = dynamic_cast<BootstrapClassLoader *>(class_loader)) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos) {
      boot->set_classpath(filepath.substr(0, pos + 1));
    }
  }

  auto initial_class = class_loader->load_class(filepath);

  RuntimeArray *args_array =
      args.empty() ? nullptr : build_args_array(this, args);

  if (auto main =
          initial_class->find_method("main", "([Ljava/lang/String;)V")) {
    auto frame = new Frame(main, initial_class);
    frame->init(main->code->max_locals, main->code->max_stack);

    if (args_array && !frame->local_vars.empty()) {
      frame->local_vars[0] =
          static_cast<Slot>(reinterpret_cast<uintptr_t>(args_array));
    }

    thread->call_stack.emplace(thread->call_stack.begin(), frame);
  }

  thread->execute_stack();
}

namespace {
std::string resolve_catch_type_name(const Frame &frame, u2 catch_type) {
  if (!frame.current_class || !frame.current_class->class_file)
    return {};
  const auto &cp = frame.current_class->class_file->constant_pool;
  if (catch_type == 0 || catch_type >= cp.size())
    return {};
  const auto &entry = cp[catch_type];
  if (entry.first != ConstantTag::CONSTANT_Class)
    return {};
  u2 name_idx = entry.second.class_info.name_index;
  return frame.current_class->class_file->resolve_utf8(name_idx);
}

bool can_handle(RuntimeObject *exc_obj, const std::string &catch_name) {
  if (catch_name.empty())
    return true;
  if (!exc_obj || !exc_obj->klass)
    return false;

  return exc_obj->klass->name == catch_name;
}
} // namespace

void Thread::execute_stack() {
  while (!this->call_stack.empty()) {
    try {
      std::cout << "Executando: " << call_stack.back()->method->name
                << std::endl;
      interpreter->execute(*call_stack.back());
    } catch (const JavaException &je) {
      RuntimeObject *exc_obj = je.object();
      bool handled = false;

      while (!call_stack.empty() && !handled) {
        Frame *frame = call_stack.back();
        const CodeAttribute *code =
            frame->method ? frame->method->code : nullptr;
        if (code) {
          for (const auto &entry : code->exception_table) {
            if (frame->pc >= entry.start_pc && frame->pc < entry.end_pc) {
              std::string catch_name =
                  resolve_catch_type_name(*frame, entry.catch_type);
              if (catch_name.empty() || can_handle(exc_obj, catch_name)) {
                frame->operand_stack.stack.clear();
                frame->operand_stack.push_ref(exc_obj);
                frame->pc = entry.handler_pc;
                handled = true;
                break;
              }
            }
          }
        }

        if (!handled) {
          pop_frame();
        }
      }

      if (!handled) {
        std::cerr << "Uncaught Java exception, terminating stack.\n";
        break;
      }
    } catch (const std::exception &e) {
      std::cerr << "Fatal runtime error: " << e.what() << std::endl;
      break;
    }
  }
}
