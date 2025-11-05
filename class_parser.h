#pragma once
#include "jvm_types.h"
#include <fstream>
#include <string>
#include <vector>

class ClassParser {
public:
  explicit ClassParser(const std::string &filepath);
  ClassFile parse();

private:
  std::ifstream file;
  ClassFile classfile;

  u1 read_u1();
  u2 read_u2();
  u4 read_u4();

  u4 readMagic();
  u2 readMinorVersion();
  u2 readMajorVersion();
  u2 readConstantPoolCount();
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
  std::vector<AttributeInfo> readAttributes(u2 count);

  std::string getUtf8(u2 index);
  VerificationTypeInfo read_verification_type_info();
  StackMapFrame read_stack_map_frame();

  ConstantPoolEntry readConstantPoolEntry();
};
