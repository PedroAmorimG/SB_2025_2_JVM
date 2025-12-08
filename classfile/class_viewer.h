#pragma once

#include "classfile_types.h"
#include <string>
#include <vector>

/**
 * @class ClassFileViewer
 * @brief Responsável pela exibição legível (disassembler) de um ClassFile.
 *
 * Esta classe recebe uma estrutura ClassFile parseada e imprime seus componentes
 * no console de forma formatada (similar ao comando `javap -v`).
 */
class ClassFileViewer {
public:
  /**
   * @brief Construtor.
   * @param cf A estrutura ClassFile a ser visualizada.
   */
  explicit ClassFileViewer(ClassFile cf);

  /**
   * @brief Método principal que imprime todas as seções do ClassFile sequencialmente.
   */
  void show_class_file();

private:
  ClassFile cf;

  // Funções de Impressão de Componentes
  
  void print_magic();
  void print_major_version();
  void print_java_version(); /// Traduz o número da versão (ex: 52 -> Java 8).
  void print_minor_version();
  
  void print_constant_pool_count();
  void print_constant_entry(u2 index); /// Imprime uma única constante formatada.
  void print_constant_pool();
  
  void print_access_flags(); /// Traduz flags de bits para texto (ex: ACC_PUBLIC).
  void print_this_class();
  void print_super_class();
  
  void print_interface_count();
  void print_interfaces();
  
  void print_field_count();
  void print_fields();
  
  void print_methods_count();
  void print_methods();
  
  void print_attribute_count();
  /** * Imprime uma lista de atributos. 
   * Identifica o tipo (Code, LineNumberTable) e formata adequadamente.
   */
  void print_attributes(u2 index, const std::vector<AttributeInfo> &entry);
  
  /** Fallback para imprimir atributos desconhecidos como bytes hexadecimais. */
  void print_attribute_info_entry(u4 index, const std::vector<u1> entry);
  
  /** * Desmontador de Bytecode: Lê o array de opcodes e imprime mnemônicos.
   */
  void print_code_attribute(const CodeAttribute &code);

  /** Helper para obter string UTF-8 da pool dentro do viewer. */
  std::string get_utf8_from_pool(const std::vector<ConstantPoolEntry> &pool,
                                 u2 index);
};