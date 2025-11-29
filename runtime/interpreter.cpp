#include "../classfile/class_parser.h"
#include "./runtime_class_types.h"
#include "interpreter.h"
#include <cstring>
#include <cmath>


void Interpreter::execute(Frame &frame) {
  while (true) {
    const u1* code = frame.method->code->code.data();
    u1 opcode = code[frame.pc];
    auto it = opcode_table.find(opcode);
    if (it != opcode_table.end()) {
        it->second(frame);   // chama a função correta
    } else {
        throw std::runtime_error("Opcode não implementado");
    }
  }
}

Interpreter::Interpreter() {
  opcode_table = {
    {0x00, exec_nop},
    {0x01, exec_aconst_null},
    {0x02, exec_iconst_m1},
    {0x03, exec_iconst_0},
    {0x04, exec_iconst_1},
    {0x05, exec_iconst_2},
    {0x06, exec_iconst_3},
    {0x07, exec_iconst_4},
    {0x08, exec_iconst_5},
    {0x09, exec_lconst_0},
    {0x0A, exec_lconst_1},
    {0x0B, exec_fconst_0},
    {0x0C, exec_fconst_1},
    {0x0D, exec_fconst_2},
    {0x0E, exec_dconst_0},
    {0x0F, exec_dconst_1},
    {0x10, exec_bipush},
    {0x11, exec_sipush},
    {0x12, exec_ldc},
    {0x13, exec_ldc_w},
    {0x14, exec_ldc2_w},
    {0x15, exec_iload},
    {0x16, exec_lload},
    {0x17, exec_fload},
    {0x18, exec_dload},
    {0x19, exec_aload},
    {0x1A, exec_iload_0},
    {0x1B, exec_iload_1},
    {0x1C, exec_iload_2},
    {0x1D, exec_iload_3},
    {0x1E, exec_lload_0},
    {0x1F, exec_lload_1},
    {0x20, exec_lload_2},
    {0x21, exec_lload_3},
    {0x22, exec_fload_0},
    {0x23, exec_fload_1},
    {0x24, exec_fload_2},
    {0x25, exec_fload_3},
    {0x26, exec_dload_0},
    {0x27, exec_dload_1},
    {0x28, exec_dload_2},
    {0x29, exec_dload_3},
    {0x2A, exec_aload_0},
    {0x2B, exec_aload_1},
    {0x2C, exec_aload_2},
    {0x2D, exec_aload_3},
    {0x2E, exec_iaload},
    {0x2F, exec_laload},
    {0x30, exec_faload},
    {0x31, exec_daload},
    {0x32, exec_aaload},
    {0x33, exec_baload},
    {0x34, exec_caload},
    {0x35, exec_saload},
    {0x36, exec_istore},
    {0x37, exec_lstore},
    {0x38, exec_fstore},
    {0x39, exec_dstore},
    {0x3A, exec_astore},
    {0x3B, exec_istore_0},
    {0x3C, exec_istore_1},
    {0x3D, exec_istore_2},
    {0x3E, exec_istore_3},
    {0x3F, exec_lstore_0},
    {0x40, exec_lstore_1},
    {0x41, exec_lstore_2},
    {0x42, exec_lstore_3},
    {0x43, exec_fstore_0},
    {0x44, exec_fstore_1},
    {0x45, exec_fstore_2},
    {0x46, exec_fstore_3},
    {0x47, exec_dstore_0},
    {0x48, exec_dstore_1},
    {0x49, exec_dstore_2},
    {0x4A, exec_dstore_3},
    {0x4B, exec_astore_0},
    {0x4C, exec_astore_1},
    {0x4D, exec_astore_2},
    {0x4E, exec_astore_3},
    {0x4F, exec_iastore},
    {0x50, exec_lastore},
    {0x51, exec_fastore},
    {0x52, exec_dastore},
    {0x53, exec_aastore},
    {0x54, exec_bastore},
    {0x55, exec_castore},
    {0x56, exec_sastore},
    {0x57, exec_pop},
    {0x58, exec_pop2},
    {0x59, exec_dup},
    {0x5A, exec_dup_x1},
    {0x5B, exec_dup_x2},
    {0x5C, exec_dup2},
    {0x5D, exec_dup2_x1},
    {0x5E, exec_dup2_x2},
    {0x5F, exec_swap},
    {0x60, exec_iadd},
    {0x61, exec_ladd},
    {0x62, exec_fadd},
    {0x63, exec_dadd},
    {0x64, exec_isub},
    {0x65, exec_lsub},
    {0x66, exec_fsub},
    {0x67, exec_dsub},
    {0x68, exec_imul},
    {0x69, exec_lmul},
    {0x6A, exec_fmul},
    {0x6B, exec_dmul},
    {0x6C, exec_idiv},
    {0x6D, exec_ldiv},
    {0x6E, exec_fdiv},
    {0x6F, exec_ddiv},
    {0x70, exec_irem},
    {0x71, exec_lrem},
    {0x72, exec_frem},
    {0x73, exec_drem},
    {0x74, exec_ineg},
    {0x75, exec_lneg},
    {0x76, exec_fneg},
    {0x77, exec_dneg},
    {0x78, exec_ishl},
    {0x79, exec_lshl},
    {0x7A, exec_ishr},
    {0x7B, exec_lshr},
    {0x7C, exec_iushr},
    {0x7D, exec_lushr},
    {0x7E, exec_iand},
    {0x7F, exec_land},
    {0x80, exec_ior},
    {0x81, exec_lor},
    {0x82, exec_ixor},
    {0x83, exec_lxor},
    {0x84, exec_iinc},
    {0x85, exec_i2l},
    {0x86, exec_i2f},
    {0x87, exec_i2d},
    {0x88, exec_l2i},
    {0x89, exec_l2f},
    {0x8A, exec_l2d},
    {0x8B, exec_f2i},
    {0x8C, exec_f2l},
    {0x8D, exec_f2d},
    {0x8E, exec_d2i},
    {0x8F, exec_d2l},
    {0x90, exec_d2f},
    {0x91, exec_i2b},
    {0x92, exec_i2c},
    {0x93, exec_i2s},
    {0x94, exec_lcmp},
    {0x95, exec_fcmpl},
    {0x96, exec_fcmpg},
    {0x97, exec_dcmpl},
    {0x98, exec_dcmpg},
    {0x99, exec_ifeq},
    {0x9A, exec_ifne},
    {0x9B, exec_iflt},
    {0x9C, exec_ifge},
    {0x9D, exec_ifgt},
    {0x9E, exec_ifle},
    {0x9F, exec_if_icmpeq},
    {0xA0, exec_if_icmpne},
    {0xA1, exec_if_icmplt},
    {0xA2, exec_if_icmpge},
    {0xA3, exec_if_icmpgt},
    {0xA4, exec_if_icmple},
    {0xA5, exec_if_acmpeq},
    {0xA6, exec_if_acmpne},
    {0xA7, exec_goto},
    {0xA8, exec_jsr},
    {0xA9, exec_ret},
    {0xAA, exec_tableswitch},
    {0xAB, exec_lookupswitch},
    {0xAC, exec_ireturn},
    {0xAD, exec_lreturn},
    {0xAE, exec_freturn},
    {0xAF, exec_dreturn},
    {0xB0, exec_areturn},
    {0xB1, exec_return},
    {0xB2, exec_getstatic},
    {0xB3, exec_putstatic},
    {0xB4, exec_getfield},
    {0xB5, exec_putfield},
    {0xB6, exec_invokevirtual},         // ve se realmente vai fazer
    {0xB7, exec_invokespecial},
    {0xB8, exec_invokestatic},
    {0xB9, exec_invokeinterface},
    {0xBA, exec_invokedynamic},
    {0xBB, exec_new},
    {0xBC, exec_newarray},
    {0xBD, exec_anewarray},
    {0xBE, exec_arraylength},
    {0xBF, exec_athrow},
    {0xC0, exec_checkcast},
    {0xC1, exec_instanceof},
    {0xC2, exec_monitorenter},
    {0xC3, exec_monitorexit},
    {0xC4, exec_wide},
    {0xC5, exec_multianewarray},
    {0xC6, exec_ifnull},
    {0xC7, exec_ifnonnull},
    {0xC8, exec_goto_w},
    {0xC9, exec_jsr_w},
  };
};

void exec_nop(Frame& frame) {frame.pc += 1;}
void exec_aconst_null(Frame& frame) {frame.operand_stack.push_ref(nullptr); frame.pc += 1;}

void exec_aload_0(Frame& frame) {
  Slot value = frame.local_vars[0];
  frame.operand_stack.stack.push_back(value);
  frame.pc += 1;
}


void exec_iconst_m1(Frame& frame) { frame.operand_stack.push_int(-1); frame.pc++; }
void exec_iconst_0(Frame& frame)  { frame.operand_stack.push_int(0);  frame.pc++; }
void exec_iconst_1(Frame& frame)  { frame.operand_stack.push_int(1);  frame.pc++; }
void exec_iconst_2(Frame& frame)  { frame.operand_stack.push_int(2);  frame.pc++; }
void exec_iconst_3(Frame& frame)  { frame.operand_stack.push_int(3);  frame.pc++; }
void exec_iconst_4(Frame& frame)  { frame.operand_stack.push_int(4);  frame.pc++; }
void exec_iconst_5(Frame& frame)  { frame.operand_stack.push_int(5);  frame.pc++; }

void exec_lconst_0(Frame& frame) { frame.operand_stack.push_long(0); frame.pc++; }
void exec_lconst_1(Frame& frame) { frame.operand_stack.push_long(1); frame.pc++; }

void exec_fconst_0(Frame& frame) { frame.operand_stack.push_float(0.0f); frame.pc++; }
void exec_fconst_1(Frame& frame) { frame.operand_stack.push_float(1.0f); frame.pc++; }
void exec_fconst_2(Frame& frame) { frame.operand_stack.push_float(2.0f); frame.pc++; }

void exec_dconst_0(Frame& frame) { frame.operand_stack.push_double(0.0); frame.pc++; }
void exec_dconst_1(Frame& frame) { frame.operand_stack.push_double(1.0); frame.pc++; }

void exec_bipush(Frame& frame) {
  u1 val = (u1)frame.method->code->code[frame.pc+1];
  frame.operand_stack.push_int(val);
  frame.pc += 2;
}

void exec_sipush(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u2 val = (code[frame.pc+1] << 8) | code[frame.pc+2];
  frame.operand_stack.push_int(val);
  frame.pc += 3;
}

void exec_ldc(Frame& frame) {
  u1 index = frame.method->code->code[frame.pc++];
  auto &entry = frame.current_class->class_file->constant_pool[index];
    switch(entry.first) {
        case ConstantTag::CONSTANT_Integer:
            frame.operand_stack.push_int(entry.second.integer_info.bytes);
            break;

        case ConstantTag::CONSTANT_Float:
            frame.operand_stack.push_float(entry.second.float_info.bytes);
            break;

        case ConstantTag::CONSTANT_String:
            break;
        /*std::string frase = frame.current_class->class_file->resolve_utf8(index);
            RuntimeObject* rf = 
            frame.operand_stack.push_ref(rf);
            break;
        */
        default:
            throw std::runtime_error("ldc: constant type not supported");
    }
}

void exec_ldc_w(Frame& frame) {}
void exec_ldc2_w(Frame& frame) {}

void exec_iload(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u1 idx = code[frame.pc+1];
  frame.operand_stack.push_int(frame.local_vars[idx]);
  frame.pc += 2;
}

void exec_lload(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u2 idx = code[frame.pc+1];
  u4 high = frame.local_vars[idx];
  u4 low  = frame.local_vars[idx + 1];
  int64_t value = ((int64_t)high << 32) | low;
  frame.operand_stack.push_long(value);
  frame.pc += 2;
}

void exec_fload(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u1 idx = code[frame.pc+1];
  frame.operand_stack.push_float(frame.local_vars[idx]);
  frame.pc += 2;
}

void exec_dload(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u2 idx = code[frame.pc+1];
  u4 high = frame.local_vars[idx];
  u4 low  = frame.local_vars[idx + 1];
  int64_t bits = ((int64_t)high << 32) | low;
  double value;
  std::memcpy(&value, &bits, sizeof(double));
  frame.operand_stack.push_long(value);
  frame.pc += 2;
}

void exec_aload(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    u1 idx = code[frame.pc+1];
    frame.operand_stack.stack.push_back(frame.local_vars[idx]);
    frame.pc += 2;
}

void exec_iload_0(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[0]); frame.pc++; }
void exec_iload_1(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[1]); frame.pc++; }
void exec_iload_2(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[2]); frame.pc++; }
void exec_iload_3(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[3]); frame.pc++; }
void exec_lload_3(Frame& frame) { frame.operand_stack.push_long(frame.local_vars[0]); frame.pc++; }
void exec_lload_3(Frame& frame) { frame.operand_stack.push_long(frame.local_vars[1]); frame.pc++; }
void exec_aload_0(Frame& frame) { frame.operand_stack.stack.push_back(frame.local_vars[0]); frame.pc++; }
void exec_aload_1(Frame& frame) { frame.operand_stack.stack.push_back(frame.local_vars[1]); frame.pc++; }
void exec_aload_2(Frame& frame) { frame.operand_stack.stack.push_back(frame.local_vars[2]); frame.pc++; }
void exec_aload_3(Frame& frame) { frame.operand_stack.stack.push_back(frame.local_vars[3]); frame.pc++; }

void exec_iaload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("iaload: null array reference");
    int32_t value;
    std::memcpy(&value, &array_ref->data[index * sizeof(int32_t)], sizeof(int32_t));// olhar dps
    frame.operand_stack.push_int(value);
    frame.pc++;
}

void exec_laload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("laload: null array reference");
    int64_t value = array_ref->read_field<int64_t>({0, index * sizeof(int64_t)});// olhar dps
    frame.operand_stack.push_long(value);
    frame.pc++;
}

void exec_faload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("faload: null array reference");
    float value = array_ref->read_field<float>({0, index * sizeof(float)});// olhar dps
    frame.operand_stack.push_float(value);
    frame.pc++;
}

void exec_daload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("daload: null array reference");
    double value = array_ref->read_field<double>({0, index * sizeof(double)});// olhar dps
    frame.operand_stack.push_double(value);
    frame.pc++;
}

void exec_aaload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("aaload: null array reference");
    RuntimeObject* value = array_ref->read_field<RuntimeObject*>({0, index * sizeof(RuntimeObject*)});// olhar dps
    frame.operand_stack.push_ref(value);
    frame.pc++;
}

void exec_baload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("baload: null array reference");
    uint8_t value = array_ref->read_field<uint8_t>({0, index * sizeof(uint8_t)});// olhar dps
    frame.operand_stack.push_int(value);
    frame.pc++;
}

void exec_caload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("caload: null array reference");
    uint16_t value = array_ref->read_field<uint16_t>({0, index * sizeof(uint16_t)});// olhar dps
    frame.operand_stack.push_int(value); // char também promovido para int
    frame.pc++;
}

void exec_saload(Frame& frame) {
    int32_t index = frame.operand_stack.pop_int();
    RuntimeObject* array_ref = frame.operand_stack.pop_ref();
    if (!array_ref) throw std::runtime_error("saload: null array reference");
    int16_t value = array_ref->read_field<int16_t>({0, index * sizeof(int16_t)});// olhar dps
    frame.operand_stack.push_int(value); // short promovido para int
    frame.pc++;
}

// STORES
void exec_istore(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    u1 idx = code[frame.pc+1];
    frame.local_vars[idx] = frame.operand_stack.pop_int();
    frame.pc += 2;
}

void exec_astore(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    u1 idx = code[frame.pc+1];
    RuntimeObject* ref = frame.operand_stack.pop_ref();
    frame.local_vars[idx] = (Slot)(uintptr_t)ref;
    frame.pc += 2;
}

void exec_istore_0(Frame& frame){ 
  frame.local_vars[0] = frame.operand_stack.pop_int();
  frame.pc++; }
void exec_lstore(Frame& frame) { 
  const u1* code = frame.method->code->code.data();
  u1 idx = code[frame.pc+1];
  frame.local_vars[idx] = frame.operand_stack.pop_long(); 
  frame.pc++; 
}

void exec_fstore(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u1 index = frame.method->code->code[frame.pc + 1];
  float value = frame.operand_stack.pop_float();
  frame.local_vars[index] = value;
  frame.pc += 2;
}

void exec_dstore(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u1 index = frame.method->code->code[frame.pc + 1];
  double value = frame.operand_stack.pop_double();
  frame.local_vars[index] = value;
  frame.pc += 2;
}

void exec_astore(Frame& frame) {
  const u1* code = frame.method->code->code.data();
  u1 index = frame.method->code->code[frame.pc + 1];
  u4 ref = frame.operand_stack.stack.back();
  frame.operand_stack.pop_int();
  frame.local_vars[index] = ref;
  frame.pc += 2;
}

void exec_istore_0(Frame& frame) {
  frame.local_vars[0] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_istore_1(Frame& frame) {
  frame.local_vars[1] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_istore_2(Frame& frame) {
  frame.local_vars[2] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_istore_3(Frame& frame) {
  frame.local_vars[3] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_lstore_0(Frame& frame) {
  frame.local_vars[0] = frame.operand_stack.pop_long();
  frame.pc++;
}

void exec_lstore_1(Frame& frame) {
  frame.local_vars[1] = frame.operand_stack.pop_long();
  frame.pc++;
}

void exec_lstore_2(Frame& frame) {
  frame.local_vars[2] = frame.operand_stack.pop_long();
  frame.pc++;
}

void exec_lstore_3(Frame& frame) {
  frame.local_vars[3] = frame.operand_stack.pop_long();
  frame.pc++;
}

void exec_fstore_0(Frame& frame) {
  frame.local_vars[0] = frame.operand_stack.pop_float();
  frame.pc++;
}

void exec_fstore_1(Frame& frame) {
  frame.local_vars[1] = frame.operand_stack.pop_float();
  frame.pc++;
}

void exec_fstore_2(Frame& frame) {
  frame.local_vars[2] = frame.operand_stack.pop_float();
  frame.pc++;
}

void exec_fstore_3(Frame& frame) {
  frame.local_vars[3] = frame.operand_stack.pop_float();
  frame.pc++;
}

void exec_dstore_0(Frame& frame) {
  frame.local_vars[0] = frame.operand_stack.pop_double();
  frame.pc++;
}

void exec_dstore_1(Frame& frame) {
  frame.local_vars[1] = frame.operand_stack.pop_double();
  frame.pc++;
}

void exec_dstore_2(Frame& frame) {
  frame.local_vars[2] = frame.operand_stack.pop_double();
  frame.pc++;
}

void exec_dstore_3(Frame& frame) {
  frame.local_vars[3] = frame.operand_stack.pop_double();
  frame.pc++;
}

void exec_astore_0(Frame& frame){
  RuntimeObject* ref = frame.operand_stack.pop_ref();
  frame.local_vars[0] = (Slot)(uintptr_t)ref; 
  frame.pc++;
}

void exec_astore_1(Frame& frame) {
  RuntimeObject* ref = frame.operand_stack.pop_ref();
  frame.local_vars[1] = (Slot)(uintptr_t)ref; 
  frame.pc++;
}
void exec_astore_2(Frame& frame) {
  RuntimeObject* ref = frame.operand_stack.pop_ref();
  frame.local_vars[2] = (Slot)(uintptr_t)ref; 
  frame.pc++;
}
void exec_astore_3(Frame& frame) {
  RuntimeObject* ref = frame.operand_stack.pop_ref();
  frame.local_vars[3] = (Slot)(uintptr_t)ref; 
  frame.pc++;
}
void exec_iastore(Frame& frame) {}
void exec_lastore(Frame& frame) {}
void exec_fastore(Frame& frame) {}
void exec_dastore(Frame& frame) {}
void exec_aastore(Frame& frame) {}
void exec_bastore(Frame& frame) {}
void exec_castore(Frame& frame) {}
void exec_sastore(Frame& frame) {}

// STACK OPS
void exec_pop(Frame& frame)   { frame.operand_stack.stack.pop_back(); frame.pc++; }
void exec_pop2(Frame& frame)  {
  frame.operand_stack.stack.pop_back(); 
  frame.operand_stack.stack.pop_back(); 
  frame.pc++; 
}

void exec_dup(Frame& frame) {
  Slot v = frame.operand_stack.stack.back();
  frame.operand_stack.stack.push_back(v);
  frame.pc++;
}

void exec_dup_x1 (Frame& frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;
}
void exec_dup_x2 (Frame& frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  Slot v3 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v3);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;

}
void exec_dup2 (Frame& frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;

}
void exec_dup2_x1 (Frame& frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  Slot v3 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v3);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;
}
void exec_dup2_x2 (Frame& frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  Slot v3 = frame.operand_stack.pop_int();
  Slot v4 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v4);
  frame.operand_stack.push_int(v3);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;
}

void exec_swap(Frame& frame) {
    u4 v1 = frame.operand_stack.pop_int();
    u4 v2 = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(v1);
    frame.operand_stack.push_int(v2);
    frame.pc += 1;
}

void exec_iadd(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a + b);
    frame.pc++;
}

void exec_ladd(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(v1 + v2);
    frame.pc++;
}

void exec_fadd(Frame& frame) {
    float v2 = frame.operand_stack.pop_float();
    float v1 = frame.operand_stack.pop_float();
    frame.operand_stack.push_float(v1 + v2);
    frame.pc++;
}

void exec_dadd(Frame& frame) {
    double v2 = frame.operand_stack.pop_double();
    double v1 = frame.operand_stack.pop_double();
    frame.operand_stack.push_double(v1 + v2);
    frame.pc++;
}

void exec_isub(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a - b);
    frame.pc++;
}

void exec_lsub(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(v1 - v2);
    frame.pc++;
}

void exec_fsub(Frame& frame) {
    float v2 = frame.operand_stack.pop_float();
    float v1 = frame.operand_stack.pop_float();
    frame.operand_stack.push_float(v1 - v2);
    frame.pc++;
}

void exec_dsub(Frame& frame) {
    double v2 = frame.operand_stack.pop_double();
    double v1 = frame.operand_stack.pop_double();
    frame.operand_stack.push_double(v1 - v2);
    frame.pc++;
}


void exec_imul(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a * b);
    frame.pc++;
}

void exec_lmul(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(v1 * v2);
    frame.pc++;
}

void exec_fmul(Frame& frame) {
    float v2 = frame.operand_stack.pop_float();
    float v1 = frame.operand_stack.pop_float();
    frame.operand_stack.push_float(v1 * v2);
    frame.pc++;
}

void exec_dmul(Frame& frame) {
    double v2 = frame.operand_stack.pop_double();
    double v1 = frame.operand_stack.pop_double();
    frame.operand_stack.push_double(v1 * v2);
    frame.pc++;
}

void exec_idiv(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    if (v2 == 0) throw std::runtime_error("ArithmeticException: / by zero");
    frame.operand_stack.push_int(v1 / v2);
    frame.pc++;
}

void exec_ldiv(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    if (v2 == 0) throw std::runtime_error("ArithmeticException: / by zero");
    frame.operand_stack.push_long(v1 / v2);
    frame.pc++;
}

void exec_fdiv(Frame& frame) {
    float v2 = frame.operand_stack.pop_float();
    float v1 = frame.operand_stack.pop_float();
    frame.operand_stack.push_float(v1 / v2);
    frame.pc++;
}

void exec_ddiv(Frame& frame) {
    double v2 = frame.operand_stack.pop_double();
    double v1 = frame.operand_stack.pop_double();
    frame.operand_stack.push_double(v1 / v2);
    frame.pc++;
}

void exec_irem(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    if (v2 == 0) throw std::runtime_error("ArithmeticException: / by zero");
    frame.operand_stack.push_int(v1 % v2);
    frame.pc++;
}

void exec_lrem(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    if (v2 == 0) throw std::runtime_error("ArithmeticException: / by zero");
    frame.operand_stack.push_long(v1 % v2);
    frame.pc++;
}

void exec_frem(Frame& frame) {
    float v2 = frame.operand_stack.pop_float();
    float v1 = frame.operand_stack.pop_float();
    frame.operand_stack.push_float(std::fmod(v1, v2));
    frame.pc++;
}

void exec_drem(Frame& frame) {
    double v2 = frame.operand_stack.pop_double();
    double v1 = frame.operand_stack.pop_double();
    frame.operand_stack.push_double(std::fmod(v1, v2));
    frame.pc++;
}

void exec_ineg(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(-v);
    frame.pc++;
}

void exec_lneg(Frame& frame) {
    int64_t v = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(-v);
    frame.pc++;
}

void exec_fneg(Frame& frame) {
    float v = frame.operand_stack.pop_float();
    frame.operand_stack.push_float(-v);
    frame.pc++;
}

void exec_dneg(Frame& frame) {
    double v = frame.operand_stack.pop_double();
    frame.operand_stack.push_double(-v);
    frame.pc++;
}

void exec_ishl(Frame& frame) {
    int32_t shift = frame.operand_stack.pop_int() & 0x1F;
    int32_t value = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(value << shift);
    frame.pc++;
}

void exec_lshl(Frame& frame) {
    int32_t shift = frame.operand_stack.pop_int() & 0x3F;
    int64_t value = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(value << shift);
    frame.pc++;
}

void exec_ishr(Frame& frame) {
    int32_t shift = frame.operand_stack.pop_int() & 0x1F;
    int32_t value = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(value >> shift);
    frame.pc++;
}

void exec_lshr(Frame& frame) {
    int32_t shift = frame.operand_stack.pop_int() & 0x3F;
    int64_t value = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(value >> shift);
    frame.pc++;
}

void exec_iushr(Frame& frame) {
    int32_t shift = frame.operand_stack.pop_int() & 0x1F;
    uint32_t value = static_cast<uint32_t>(frame.operand_stack.pop_int());
    frame.operand_stack.push_int(static_cast<int32_t>(value >> shift));
    frame.pc++;
}

void exec_lushr(Frame& frame) {
    int32_t shift = frame.operand_stack.pop_int() & 0x3F;
    uint64_t value = static_cast<uint64_t>(frame.operand_stack.pop_long());
    frame.operand_stack.push_long(static_cast<int64_t>(value >> shift));
    frame.pc++;
}

void exec_iand(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(v1 & v2);
    frame.pc++;
}

void exec_land(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(v1 & v2);
    frame.pc++;
}

void exec_ior(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(v1 | v2);
    frame.pc++;
}

void exec_lor(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(v1 | v2);
    frame.pc++;
}

void exec_ixor(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(v1 ^ v2);
    frame.pc++;
}

void exec_lxor(Frame& frame) {
    int64_t v2 = frame.operand_stack.pop_long();
    int64_t v1 = frame.operand_stack.pop_long();
    frame.operand_stack.push_long(v1 ^ v2);
    frame.pc++;
}

void exec_iinc(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    u1 index = code[frame.pc + 1];
    int8_t constant = static_cast<int8_t>(code[frame.pc + 2]);

    int32_t value = static_cast<int32_t>(frame.local_vars[index]);
    value += constant;
    frame.local_vars[index] = static_cast<u4>(value);

    frame.pc += 3;
}

void exec_i2l(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    frame.operand_stack.push_long(static_cast<int64_t>(v));
    frame.pc++;
}

void exec_i2f(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    frame.operand_stack.push_float(static_cast<float>(v));
    frame.pc++;
}

void exec_i2d(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    frame.operand_stack.push_double(static_cast<double>(v));
    frame.pc++;
}

void exec_l2i(Frame& frame) {
    int64_t v = frame.operand_stack.pop_long();
    frame.operand_stack.push_int(static_cast<int32_t>(v));
    frame.pc++;
}

void exec_l2f(Frame& frame) {
    int64_t v = frame.operand_stack.pop_long();
    frame.operand_stack.push_float(static_cast<float>(v));
    frame.pc++;
}

void exec_l2d(Frame& frame) {
    int64_t v = frame.operand_stack.pop_long();
    frame.operand_stack.push_double(static_cast<double>(v));
    frame.pc++;
}

void exec_f2i(Frame& frame) {
    float v = frame.operand_stack.pop_float();
    frame.operand_stack.push_int(static_cast<int32_t>(v));
    frame.pc++;
}

void exec_f2l(Frame& frame) {
    float v = frame.operand_stack.pop_float();
    frame.operand_stack.push_long(static_cast<int64_t>(v));
    frame.pc++;
}

void exec_f2d(Frame& frame) {
    float v = frame.operand_stack.pop_float();
    frame.operand_stack.push_double(static_cast<double>(v));
    frame.pc++;
}

void exec_d2i(Frame& frame) {
    double v = frame.operand_stack.pop_double();
    frame.operand_stack.push_int(static_cast<int32_t>(v));
    frame.pc++;
}

void exec_d2l(Frame& frame) {
    double v = frame.operand_stack.pop_double();
    frame.operand_stack.push_long(static_cast<int64_t>(v));
    frame.pc++;
}

void exec_d2f(Frame& frame) {
    double v = frame.operand_stack.pop_double();
    frame.operand_stack.push_float((float)v);
    frame.pc += 1;
}

void exec_i2b(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int8_t r = (int8_t)v;  
    frame.operand_stack.push_int((int32_t)r);
    frame.pc += 1;
}

void exec_i2c(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    uint16_t r = (uint16_t)v;
    frame.operand_stack.push_int((int32_t)r);
    frame.pc += 1;
}

void exec_i2s(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t r = (int16_t)v;
    frame.operand_stack.push_int((int32_t)r);
    frame.pc += 1;
}

void exec_lcmp(Frame& frame) {
    int64_t b = frame.operand_stack.pop_long();
    int64_t a = frame.operand_stack.pop_long();
    int32_t r = (a == b ? 0 : (a > b ? 1 : -1));
    frame.operand_stack.push_int(r);
    frame.pc += 1;
}

void exec_fcmpl(Frame& frame) {
    float b = frame.operand_stack.pop_float();
    float a = frame.operand_stack.pop_float();
    int32_t r;

    if (std::isnan(a) || std::isnan(b)) r = -1;
    else r = (a == b ? 0 : (a > b ? 1 : -1));

    frame.operand_stack.push_int(r);
    frame.pc += 1;
}

void exec_fcmpg(Frame& frame) {
    float b = frame.operand_stack.pop_float();
    float a = frame.operand_stack.pop_float();
    int32_t r;
    if (std::isnan(a) || std::isnan(b)) r = 1;
    else r = (a == b ? 0 : (a > b ? 1 : -1));
    frame.operand_stack.push_int(r);
    frame.pc += 1;
}

void exec_dcmpl(Frame& frame) {
    double b = frame.operand_stack.pop_double();
    double a = frame.operand_stack.pop_double();
    int32_t r;
    if (std::isnan(a) || std::isnan(b)) r = -1;
    else r = (a == b ? 0 : (a > b ? 1 : -1));
    frame.operand_stack.push_int(r);
    frame.pc += 1;
}

void exec_dcmpg(Frame& frame) {
    double b = frame.operand_stack.pop_double();
    double a = frame.operand_stack.pop_double();
    int32_t r;

    if (std::isnan(a) || std::isnan(b)) r = 1;
    else r = (a == b ? 0 : (a > b ? 1 : -1));

    frame.operand_stack.push_int(r);
    frame.pc += 1;
}


static inline int16_t read_s16(Frame& frame) {
    u1 b1 = frame.method->code->code[frame.pc + 1];
    u1 b2 = frame.method->code->code[frame.pc + 2];
    return (int16_t)((b1 << 8) | b2);
}

void exec_ifeq(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v == 0 ? off : 3);
}

void exec_ifne(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v != 0 ? off : 3);
}

void exec_iflt(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v < 0 ? off : 3);
}

void exec_ifge(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v >= 0 ? off : 3);
}

void exec_ifgt(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v > 0 ? off : 3);
}

void exec_ifle(Frame& frame) {
    int32_t v = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v <= 0 ? off : 3);
}


void exec_if_icmpeq(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v1 == v2 ? off : 3);
}

void exec_if_icmpne(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v1 != v2 ? off : 3);
}

void exec_if_icmplt(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v1 < v2 ? off : 3);
}

void exec_if_icmpge(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v1 >= v2 ? off : 3);
}

void exec_if_icmpgt(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v1 > v2 ? off : 3);
}

void exec_if_icmple(Frame& frame) {
    int32_t v2 = frame.operand_stack.pop_int();
    int32_t v1 = frame.operand_stack.pop_int();
    int16_t off = read_s16(frame);
    frame.pc += (v1 <= v2 ? off : 3);
}


void exec_if_acmpeq(Frame& frame) {
    u4 ref2 = (u4)frame.operand_stack.pop_ref();
    u4 ref1 = (u4)frame.operand_stack.pop_ref();
    int16_t off = read_s16(frame);
    frame.pc += (ref1 == ref2 ? off : 3);
}

void exec_if_acmpne(Frame& frame) {
    u4 ref2 = (u4)frame.operand_stack.pop_ref();
    u4 ref1 = (u4)frame.operand_stack.pop_ref();
    int16_t off = read_s16(frame);
    frame.pc += (ref1 != ref2 ? off : 3);
}

void exec_goto(Frame& frame) {
    int16_t off = read_s16(frame);
    frame.pc += off;  // sempre pula
}

// JSR empilha o endereço de retorno (pc + 3)
// e então salta para offset
void exec_jsr(Frame& frame) {
    int16_t off = read_s16(frame);
    int32_t return_addr = frame.pc + 3;
    frame.operand_stack.push_int(return_addr);
    frame.pc += off;
}

void exec_ret(Frame& frame) {
    u1 index = frame.method->code->code[frame.pc + 1];
    int32_t addr = frame.local_vars[index];
    frame.pc = addr;
}


void exec_tableswitch(Frame& frame) {
    const u1* code = frame.method->code->code.data();

    // Alinhar para múltiplo de 4 após o opcode
    u4 base = frame.pc + 1;
    while (base % 4 != 0) base++;

    // Lê default, low e high (todos u4 big-endian)
    auto read_u4 = [&](u4 pos) {
        return (code[pos] << 24) | (code[pos+1] << 16) |
               (code[pos+2] << 8) | code[pos+3];
    };

    int32_t key = frame.operand_stack.pop_int();

    u4 default_offset = read_u4(base);
    int32_t low       = static_cast<int32_t>(read_u4(base + 4));
    int32_t high      = static_cast<int32_t>(read_u4(base + 8));

    u4 entries_start = base + 12;

    if (key < low || key > high) {
        frame.pc += static_cast<int32_t>(default_offset);
        return;
    }

    u4 index = key - low;
    u4 offset_pos = entries_start + index * 4;
    u4 jump_offset = read_u4(offset_pos);

    frame.pc += static_cast<int32_t>(jump_offset);
}

void exec_lookupswitch(Frame& frame) {
    const u1* code = frame.method->code->code.data();

    u4 base = frame.pc + 1;
    while (base % 4 != 0) base++;

    auto read_u4 = [&](u4 pos) {
        return (code[pos] << 24) | (code[pos+1] << 16) |
               (code[pos+2] << 8) | code[pos+3];
    };

    int32_t key = frame.operand_stack.pop_int();

    u4 default_offset = read_u4(base);
    u4 npairs = read_u4(base + 4);

    u4 pos = base + 8;

    for (u4 i = 0; i < npairs; i++) {
        int32_t match = static_cast<int32_t>(read_u4(pos));
        u4 offset = read_u4(pos + 4);

        if (key == match) {
            frame.pc += static_cast<int32_t>(offset);
            return;
        }

        pos += 8;
    }

    frame.pc += static_cast<int32_t>(default_offset);
}

void exec_ireturn(Frame& frame) {
    int32_t value = frame.operand_stack.pop_int();
    frame.return_value = Slot(value);
    frame.finished = true;
    
}

void exec_lreturn(Frame& frame) {
    int64_t value = frame.operand_stack.pop_long();
    frame.return_value_high = static_cast<u4>((value >> 32) & 0xFFFFFFFF);
    frame.return_value_low  = static_cast<u4>(value & 0xFFFFFFFF);
    frame.finished = true;
}

void exec_freturn(Frame& frame) {
    float value = frame.operand_stack.pop_float();
    union { float f; u4 u; } conv;
    conv.f = value;
    frame.return_value = conv.u;
    frame.finished = true;
}

void exec_dreturn(Frame& frame) {
    double value = frame.operand_stack.pop_double();
    union { double d; u8 u; } conv;
    conv.d = value;
    u8 raw = conv.u;
    frame.return_value_high = static_cast<u4>((raw >> 32) & 0xFFFFFFFF);
    frame.return_value_low  = static_cast<u4>(raw & 0xFFFFFFFF);
    frame.finished = true;
}

void exec_areturn(Frame& frame) {
    RuntimeObject* ref = frame.operand_stack.pop_ref();
    frame.return_ref = ref;
    frame.finished = true;
}

void exec_return(Frame& frame) {
    frame.finished = true;
}

// ================================================================
// 0xB2 GETSTATIC
// ================================================================
void exec_getstatic(Frame& frame) {
    u1 op1 = frame.method->code->code[frame.pc + 1];
    u1 op2 = frame.method->code->code[frame.pc + 2];
    u2 index = (op1 << 8) | op2;

    RuntimeField* field = resolve_field(frame.current_class, index);
    RuntimeClass* klass = field->owner;

    // lê o valor do campo estático
    if (field->is_64bit) {
        int64_t v = klass->static_data.read_long(field->offset);
        frame.operand_stack.push_long(v);
    } else {
        u4 v = klass->static_data.read_u4(field->offset);
        frame.operand_stack.push_int(v);
    }

    frame.pc += 3;
}

// ================================================================
// 0xB3 PUTSTATIC
// ================================================================
void exec_putstatic(Frame& frame) {
    u1 op1 = frame.method->code->code[frame.pc + 1];
    u1 op2 = frame.method->code->code[frame.pc + 2];
    u2 index = (op1 << 8) | op2;

    RuntimeField* field = resolve_field(frame.current_class, index);
    RuntimeClass* klass = field->owner;

    if (field->is_64bit) {
        int64_t v = frame.operand_stack.pop_long();
        klass->static_data.write_long(field->offset, v);
    } else {
        u4 v = frame.operand_stack.pop_int();
        klass->static_data.write_u4(field->offset, v);
    }

    frame.pc += 3;
}

// ================================================================
// 0xB4 GETFIELD
// ================================================================
void exec_getfield(Frame& frame) {
    u2 index = (frame.method->code->code[frame.pc+1] << 8) |
                frame.method->code->code[frame.pc+2];

    RuntimeField* field = resolve_field(frame.current_class, index);
    RuntimeObject* obj = frame.operand_stack.pop_ref();
    if (!obj) throw std::runtime_error("getfield: null pointer");

    if (field->is_64bit) {
        int64_t v = obj->read_field<int64_t>(*field);
        frame.operand_stack.push_long(v);
    } else {
        u4 v = obj->read_field<u4>(*field);
        frame.operand_stack.push_int(v);
    }

    frame.pc += 3;
}

// ================================================================
// 0xB5 PUTFIELD
// ================================================================
void exec_putfield(Frame& frame) {
    u2 index = (frame.method->code->code[frame.pc+1] << 8) |
                frame.method->code->code[frame.pc+2];

    RuntimeField* field = resolve_field(frame.current_class, index);

    if (field->is_64bit) {
        int64_t v = frame.operand_stack.pop_long();
        RuntimeObject* obj = frame.operand_stack.pop_ref();
        if (!obj) throw std::runtime_error("putfield: null ptr");
        obj->write_field<int64_t>(*field, v);
    } else {
        u4 v = frame.operand_stack.pop_int();
        RuntimeObject* obj = frame.operand_stack.pop_ref();
        if (!obj) throw std::runtime_error("putfield: null ptr");
        obj->write_field<u4>(*field, v);
    }

    frame.pc += 3;
}

// ================================================================
// Helper genérico pra invokes
// ================================================================
Frame* invoke_method(RuntimeMethod* method, std::vector<Slot> args) {
    Frame* new_frame = new Frame();
    new_frame->method = method;
    new_frame->current_class = method->owner;
    new_frame->pc = 0;

    // carrega argumentos nos local_vars (slot 0 = this)
    new_frame->local_vars = args;

    return new_frame;
}

// ================================================================
// 0xB6 INVOKEVIRTUAL
// ================================================================
void exec_invokevirtual(Frame& frame) {
    u2 index = (frame.method->code->code[frame.pc+1] << 8) |
                frame.method->code->code[frame.pc+2];

    RuntimeMethod* method = resolve_method_virtual(frame.current_class, index);

 // número de argumentos (inclui "this")
    int arg_count = method->arg_slots;
    std::vector<Slot> args(arg_count);

    for (int i = arg_count - 1; i >= 0; --i)
        args[i] = frame.operand_stack.stack.back(),
        frame.operand_stack.stack.pop_back();

    Frame* new_frame = invoke_method(method, args);
    push_new_frame(new_frame);

    frame.pc += 3;
}

// ================================================================
// 0xB7 INVOKESPECIAL  (super, <init>, private)
// ================================================================
void exec_invokespecial(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];

    RuntimeMethod* method = resolve_method_special(frame.current_class, index);
    int arg_slots = method->arg_slots;

    std::vector<Slot> args(arg_slots);
    for (int i = arg_slots - 1; i >= 0; --i) {
        args[i] = frame.operand_stack.stack.back();
        frame.operand_stack.stack.pop_back();
    }

    Frame* new_frame = new Frame(method);
    new_frame->local_vars = args;
    push_new_frame(new_frame);

    frame.pc += 3;
}

// ================================================================
// 0xB8 INVOKESTATIC
// ================================================================
void exec_invokestatic(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];

    RuntimeMethod* method = resolve_method_static(frame.current_class, index);
    int arg_slots = method->arg_slots;

    std::vector<Slot> args(arg_slots);
    for (int i = arg_slots - 1; i >= 0; --i) {
        args[i] = frame.operand_stack.stack.back();
        frame.operand_stack.stack.pop_back();
    }

    Frame* new_frame = new Frame(method);
    new_frame->local_vars = args;
    push_new_frame(new_frame);

    frame.pc += 3;
}

// ================================================================
// 0xB9 INVOKEINTERFACE
// ================================================================
void exec_invokeinterface(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];
    u1 count = frame.code()[frame.pc+3]; // ignorado na JVM moderna

    RuntimeMethod* method = resolve_method_interface(frame.current_class, index);

    int arg_slots = method->arg_slots;
    std::vector<Slot> args(arg_slots);

    for (int i = arg_slots - 1; i >= 0; --i) {
        args[i] = frame.operand_stack.stack.back();
        frame.operand_stack.stack.pop_back();
    }

    Frame* new_frame = new Frame(method);
    new_frame->local_vars = args;
    push_new_frame(new_frame);

    frame.pc += 4;
}

// ================================================================
// 0xBA INVOKEDYNAMIC   (implementação mínima)
// ================================================================
void exec_invokedynamic(Frame& frame) {
    throw std::runtime_error("invokedynamic: not implemented in this JVM");
}

// ================================================================
// 0xBB NEW
// ================================================================
void exec_new(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];

    RuntimeClass* klass = resolve_class(frame.current_class, index);
    RuntimeObject* obj = new RuntimeObject(klass);

    frame.operand_stack.push_ref(obj);
    frame.pc += 3;
}

// ================================================================
// 0xBC NEWARRAY
// ================================================================
void exec_newarray(Frame& frame) {
    u1 atype = frame.code()[frame.pc+1];
    int32_t count = frame.operand_stack.pop_int();

    if (count < 0) throw std::runtime_error("newarray: negative size");

    RuntimeArray* arr = new RuntimeArray(atype, count);
    frame.operand_stack.push_ref(arr);

    frame.pc += 2;
}

// ================================================================
// 0xBD ANEWARRAY (array de referências)
// ================================================================
void exec_anewarray(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];
    int32_t count = frame.operand_stack.pop_int();

    if (count < 0) throw std::runtime_error("anewarray: negative size");

    RuntimeClass* klass = resolve_class(frame.current_class, index);
    RuntimeArray* arr = new RuntimeArray(klass, count);

    frame.operand_stack.push_ref(arr);
    frame.pc += 3;
}

// ================================================================
// 0xBE ARRAYLENGTH
// ================================================================
void exec_arraylength(Frame& frame) {
    RuntimeObject* ref = frame.operand_stack.pop_ref();
    if (!ref) throw std::runtime_error("arraylength: null");

    RuntimeArray* arr = dynamic_cast<RuntimeArray*>(ref);
    frame.operand_stack.push_int(arr->length);

    frame.pc += 1;
}

// ================================================================
// 0xBF ATHROW
// ================================================================
void exec_athrow(Frame& frame) {
    RuntimeObject* exception = frame.operand_stack.pop_ref();
    if (!exception) throw std::runtime_error("athrow: null");

    throw_java_exception(exception);  // depende da sua VM

    // nunca volta
}

// ================================================================
// 0xC0 CHECKCAST
// ================================================================
void exec_checkcast(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];
    RuntimeClass* target = resolve_class(frame.current_class, index);

    RuntimeObject* ref = frame.operand_stack.pop_ref();
    frame.operand_stack.push_ref(ref); // devolve

    if (ref && !is_instance_of(ref, target))
        throw std::runtime_error("checkcast: incompatible types");

    frame.pc += 3;
}

// ================================================================
// 0xC1 INSTANCEOF
// ================================================================
void exec_instanceof(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];
    RuntimeClass* target = resolve_class(frame.current_class, index);

    RuntimeObject* ref = frame.operand_stack.pop_ref();
    int res = (ref && is_instance_of(ref, target)) ? 1 : 0;

    frame.operand_stack.push_int(res);
    frame.pc += 3;
}

// ================================================================
// 0xC2 MONITORENTER
// ================================================================
void exec_monitorenter(Frame& frame) {
    RuntimeObject* ref = frame.operand_stack.pop_ref();
    if (!ref) throw std::runtime_error("monitorenter: null");

    // implementação mínima: ignore (single-thread VM)
    // se quiser lock real, adiciono depois.

    frame.pc += 1;
}

// ================================================================
// 0xC3 MONITOREXIT
// ================================================================
void exec_monitorexit(Frame& frame) {
    RuntimeObject* ref = frame.operand_stack.pop_ref();
    if (!ref) throw std::runtime_error("monitorexit: null");

    // implementação mínima: ignore

    frame.pc += 1;
}

// ================================================================
// 0xC4 WIDE
// ================================================================
void exec_wide(Frame& frame) {
    u1 opcode = frame.code()[frame.pc+1];

    if (opcode == 0x84) {
        u2 index = (frame.code()[frame.pc+2] << 8) | frame.code()[frame.pc+3];
        int16_t constant = (frame.code()[frame.pc+4] << 8) | frame.code()[frame.pc+5];

        int32_t v = frame.local_vars[index];
        frame.local_vars[index] = v + constant;

        frame.pc += 6;
    } else {
        u2 index = (frame.code()[frame.pc+2] << 8) | frame.code()[frame.pc+3];
        // load/store 16-bit index
        exec_wide_loadstore(frame, opcode, index);
        frame.pc += 4;
    }
}

// ================================================================
// 0xC5 MULTIANEWARRAY
// ================================================================
void exec_multianewarray(Frame& frame) {
    u2 index = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];
    u1 dims  = frame.code()[frame.pc+3];

    std::vector<int32_t> sizes(dims);
    for (int i = dims - 1; i >= 0; i--)
        sizes[i] = frame.operand_stack.pop_int();

    RuntimeClass* klass = resolve_class(frame.current_class, index);
    RuntimeObject* arr = create_multi_array(klass, sizes);

    frame.operand_stack.push_ref(arr);
    frame.pc += 4;
}

// ================================================================
// 0xC6 IFNULL
// ================================================================
void exec_ifnull(Frame& frame) {
    int16_t offset = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];

    RuntimeObject* ref = frame.operand_stack.pop_ref();
    if (ref == nullptr)
        frame.pc += offset;
    else
        frame.pc += 3;
}

// ================================================================
// 0xC7 IFNONNULL
// ================================================================
void exec_ifnonnull(Frame& frame) {
    int16_t offset = (frame.code()[frame.pc+1] << 8) | frame.code()[frame.pc+2];

    RuntimeObject* ref = frame.operand_stack.pop_ref();
    if (ref != nullptr)
        frame.pc += offset;
    else
        frame.pc += 3;
}

// ================================================================
// 0xC8 GOTO_W
// ================================================================
void exec_goto_w(Frame& frame) {
    int32_t offset =
        (frame.code()[frame.pc+1] << 24) |
        (frame.code()[frame.pc+2] << 16) |
        (frame.code()[frame.pc+3] << 8)  |
         frame.code()[frame.pc+4];

    frame.pc += offset;
}

// ================================================================
// 0xC9 JSR_W
// ================================================================
void exec_jsr_w(Frame& frame) {
    int32_t offset =
        (frame.code()[frame.pc+1] << 24) |
        (frame.code()[frame.pc+2] << 16) |
        (frame.code()[frame.pc+3] << 8)  |
         frame.code()[frame.pc+4];

    frame.operand_stack.push_int(frame.pc + 5);
    frame.pc += offset;
}
