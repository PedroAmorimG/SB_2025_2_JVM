#pragma once
#include "classfile_types.h"
#include <fstream>
#include <string>
#include <vector>

/**
 * @class ClassParser
 * @brief Responsável pela leitura e análise (parsing) do arquivo binário .class.
 *
 * Esta classe encapsula a lógica de abrir o arquivo, ler bytes sequencialmente
 * (respeitando o Big-Endian) e preencher a estrutura ClassFile.
 */
class ClassParser {
public:
  /**
   * @brief Construtor: Abre o arquivo .class.
   * @param filepath Caminho do arquivo a ser lido.
   */
  explicit ClassParser(const std::string &filepath);

  /**
   * @brief Executa o processo de parsing completo.
   * @return Uma estrutura ClassFile preenchida com todos os dados do arquivo.
   */
  ClassFile parse();

private:
  std::ifstream file;
  ClassFile classfile;

  // Funções Auxiliares de Leitura de Bytes 
  u1 read_u1(); ///< Lê 1 byte.
  u2 read_u2(); ///< Lê 2 bytes (big-endian para host).
  u4 read_u4(); ///< Lê 4 bytes (big-endian para host).

  // Funções de Leitura das Seções do ClassFile 
  
  /** Verifica se o arquivo começa com 0xCAFEBABE. */
  u4 readMagic();
  
  u2 readMinorVersion();
  u2 readMajorVersion();
  u2 readConstantPoolCount();
  
  /** Lê todas as entradas da tabela de constantes. */
  std::vector<ConstantPoolEntry> readConstantPool(u2 count);
  
  u2 readAccessFlags();
  u2 readThisClass();
  u2 readSuperClass();
  u2 readInterfacesCount();
  std::vector<u2> readInterfaces(u2 count);
  
  u2 readFieldsCount();
  std::vector<FieldInfo> readFields(u2 count);
  
  u2 readMethodsCount();
  std::vector<MethodInfo> readMethods(u2 count);
  
  u2 readAttributesCount();
  /** * Lê os atributos (Code, SourceFile, etc). É recursivo para atributos aninhados.
   */
  std::vector<AttributeInfo> readAttributes(u2 count);

  // Helpers Internos
  
  /** Busca string UTF8 na pool já lida (usado para identificar nomes de atributos). */
  std::string getUtf8(u2 index);
  
  /** Lê informações de tipos para o StackMapTable. */
  VerificationTypeInfo read_verification_type_info();
  
  /** Lê um frame do StackMapTable. */
  StackMapFrame read_stack_map_frame();

  /** Lê uma única entrada do Constant Pool (auxiliar de readConstantPool). */
  ConstantPoolEntry readConstantPoolEntry();
};