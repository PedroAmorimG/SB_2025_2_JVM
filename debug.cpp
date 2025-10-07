#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "class_parser.h"
#include "class_viewer.h"
#include "jvm_types.h"

//g++ -std=c++17 -Wall -O2 -o debug debug.cpp

// Estrutura principal da ClassFile
struct ClassFile {
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    std::vector<ConstantPoolEntry> constant_pool;
};

int main() {
    const char* filename = "Hello.class";
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir " << filename << std::endl;
        return 1;
    }

    ClassFile cf;
    cf.magic = read_4bytes(file);
    cf.minor_version = read_2bytes(file);
    cf.major_version = read_2bytes(file);
    cf.constant_pool_count = read_2bytes(file);

    std::cout << std::hex << std::showbase;
    std::cout << "Magic: " << cf.magic << std::endl;
    std::cout << std::dec;
    std::cout << "Minor version: " << cf.minor_version << std::endl;
    std::cout << "Major version: " << cf.major_version << std::endl;
    std::cout << "Constant pool count: " << cf.constant_pool_count << std::endl;

    // Ler constant pool vazio (apenas tags, simplificado)
    cf.constant_pool.emplace_back(ConstantPoolEntry{0}); // índice 0 vazio
    for (u2 i = 1; i < cf.constant_pool_count; ++i) {
        u1 tag = read_1byte(file);
        cf.constant_pool.push_back(ConstantPoolEntry{tag});
        std::cout << "Entry " << i << " tag: " << +tag << std::endl;
    }

    file.close();
    return 0;
}