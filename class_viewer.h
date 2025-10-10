#include "jvm_types.h"

void print_magic(u4 magic);
void print_major_version(u2 major);
void print_minor_version(u2 minor);
void print_constant_pool_count(u2 count);
void print_constant_entry(u2 index, const ConstantPoolEntry &entry);
void print_access_flags(u2 flag);
void print_this_class(u2 index);
void print_super_class(u2 index);
