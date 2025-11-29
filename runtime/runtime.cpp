#include "./runtime_class_types.h"
#include <string>
#include <iostream>

void Runtime::start(std::string filepath)
{
  auto initial_class = class_loader->load_class(filepath);

  if (auto main = initial_class->find_method("main", "([Ljava/lang/String;)V"))
  {
    auto frame = new Frame(main, initial_class);
    frame->init(main->code->max_locals, main->code->max_stack);

    thread->call_stack.emplace(thread->call_stack.begin(), frame);
  }

  for (auto frame : thread->call_stack)
  {
    std::cout << frame->method->name << std::endl;
  }
}