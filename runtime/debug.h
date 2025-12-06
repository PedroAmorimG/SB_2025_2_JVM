#pragma once

#include <iostream>
#include <string>

extern bool g_debug_enabled;

#define DEBUG_LOG(msg)                                                          \
  do {                                                                          \
    if (g_debug_enabled) {                                                      \
      std::cout << msg << std::endl;                                            \
    }                                                                           \
  } while (0)
