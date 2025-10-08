#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "class_parser.h"
#include "class_viewer.h"
#include "jvm_types.h"

// g++ *.cpp -o debug ←←←
// g++ -std=c++17 -Wall -O2 -o debug debug.cpp

int main() {
    const char* filename = "Hello.class";
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir " << filename << std::endl;
        return 1;
    }

    // Chama sua função já existente
    u4 magic = read_magic(file);
    u2 major = read_major_version(file);
    u2 minor = read_minor_version(file);
    u2 count_pool = read_constant_pool_count(file);
    std::vector<ConstantPoolEntry> pool_const = read_constant_pool(file, count_pool, true);
    u2 flags = read_access_flags(file);
    u2 tsclass = read_this_class(file);
    u2 spclass = read_super_class(file);
    file.close();
    return 0;
}