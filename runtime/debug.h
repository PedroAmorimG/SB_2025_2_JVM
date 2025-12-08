#pragma once

#include <iostream>
#include <string>

/** @brief Flag global que controla a exibição de logs de debug da JVM. */
extern bool g_debug_enabled;

/**
 * @def DEBUG_LOG
 * @brief Macro para impressão condicional de mensagens de debug.
 * Se g_debug_enabled for false, o código é ignorado em tempo de execução.
 */
#define DEBUG_LOG(msg)                                                          \
  do {                                                                          \
    if (g_debug_enabled) {                                                      \
      std::cout << msg << std::endl;                                            \
    }                                                                           \
  } while (0)
