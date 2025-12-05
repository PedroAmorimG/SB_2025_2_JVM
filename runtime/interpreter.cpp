#include "interpreter.h"
#include "../classfile/class_parser.h"
#include "./array.h"
#include "./runtime_class_types.h"
#include <cmath>
#include <cstring>

void Interpreter::execute(Frame &frame) {
  while (true) {
    const u1 *code = frame.method->code->code.data();
    u1 opcode = code[frame.pc];
    auto it = opcode_table.find(opcode);
    if (it != opcode_table.end()) {
      it->second(this->thread, frame); // chama a função correta
    } else {
      throw std::runtime_error("Opcode não implementado");
    }
  }
}

Interpreter::Interpreter(Thread *t) : thread(t) {
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
      {0xB6, exec_invokevirtual},
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

void exec_nop(Thread *t, Frame &frame) { frame.pc += 1; }
void exec_aconst_null(Thread *t, Frame &frame) {
  frame.operand_stack.push_ref(nullptr);
  frame.pc += 1;
}

void exec_iconst_m1(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(-1);
  frame.pc++;
}
void exec_iconst_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(0);
  frame.pc++;
}
void exec_iconst_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(1);
  frame.pc++;
}
void exec_iconst_2(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(2);
  frame.pc++;
}
void exec_iconst_3(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(3);
  frame.pc++;
}
void exec_iconst_4(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(4);
  frame.pc++;
}
void exec_iconst_5(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(5);
  frame.pc++;
}

void exec_lconst_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_long(0);
  frame.pc++;
}
void exec_lconst_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_long(1);
  frame.pc++;
}

void exec_fconst_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(0.0f);
  frame.pc++;
}
void exec_fconst_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(1.0f);
  frame.pc++;
}
void exec_fconst_2(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(2.0f);
  frame.pc++;
}

void exec_dconst_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_double(0.0);
  frame.pc++;
}
void exec_dconst_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_double(1.0);
  frame.pc++;
}

void exec_bipush(Thread *t, Frame &frame) {
  u1 val = (u1)frame.method->code->code[frame.pc + 1];
  frame.operand_stack.push_int(val);
  frame.pc += 2;
}

void exec_sipush(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u2 val = (code[frame.pc + 1] << 8) | code[frame.pc + 2];
  frame.operand_stack.push_int(val);
  frame.pc += 3;
}

void exec_ldc(Thread *t, Frame &frame) {
  u1 index = frame.method->code->code[frame.pc++];
  auto &entry = frame.current_class->class_file->constant_pool[index];
  switch (entry.first) {
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

void exec_ldc_w(Thread *t, Frame &frame) {}
void exec_ldc2_w(Thread *t, Frame &frame) {}

void exec_iload(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 idx = code[frame.pc + 1];
  frame.operand_stack.push_int(frame.local_vars[idx]);
  frame.pc += 2;
}

void exec_lload(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u2 idx = code[frame.pc + 1];
  u4 high = frame.local_vars[idx];
  u4 low = frame.local_vars[idx + 1];
  int64_t value = ((int64_t)high << 32) | low;
  frame.operand_stack.push_long(value);
  frame.pc += 2;
}

void exec_fload(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 idx = code[frame.pc + 1];
  frame.operand_stack.push_float(frame.local_vars[idx]);
  frame.pc += 2;
}

void exec_dload(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u2 idx = code[frame.pc + 1];
  u4 high = frame.local_vars[idx];
  u4 low = frame.local_vars[idx + 1];
  int64_t bits = ((int64_t)high << 32) | low;
  double value;
  std::memcpy(&value, &bits, sizeof(double));
  frame.operand_stack.push_long(value);
  frame.pc += 2;
}

void exec_fload_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(frame.local_vars[0]);
  frame.pc++;
}
void exec_fload_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(frame.local_vars[1]);
  frame.pc++;
}
void exec_fload_2(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(frame.local_vars[2]);
  frame.pc++;
}
void exec_fload_3(Thread *t, Frame &frame) {
  frame.operand_stack.push_float(frame.local_vars[3]);
  frame.pc++;
}

void exec_dload_0(Thread *t, Frame &frame) {
  uint64_t bits = (static_cast<uint64_t>(frame.local_vars[0]) << 32) |
                  static_cast<uint64_t>(frame.local_vars[1]);
  double value;
  std::memcpy(&value, &bits, sizeof(double));
  frame.operand_stack.push_double(value);
  frame.pc++;
}
void exec_dload_1(Thread *t, Frame &frame) {
  uint64_t bits = (static_cast<uint64_t>(frame.local_vars[1]) << 32) |
                  static_cast<uint64_t>(frame.local_vars[2]);
  double value;
  std::memcpy(&value, &bits, sizeof(double));
  frame.operand_stack.push_double(value);
  frame.pc++;
}
void exec_dload_2(Thread *t, Frame &frame) {
  uint64_t bits = (static_cast<uint64_t>(frame.local_vars[2]) << 32) |
                  static_cast<uint64_t>(frame.local_vars[3]);
  double value;
  std::memcpy(&value, &bits, sizeof(double));
  frame.operand_stack.push_double(value);
  frame.pc++;
}
void exec_dload_3(Thread *t, Frame &frame) {
  // assumes enough locals; high part is in local_vars[3], low part undefined ->
  // push based on available slot
  uint64_t bits = static_cast<uint64_t>(frame.local_vars[3]) << 32;
  double value;
  std::memcpy(&value, &bits, sizeof(double));
  frame.operand_stack.push_double(value);
  frame.pc++;
}

void exec_aload(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 idx = code[frame.pc + 1];
  frame.operand_stack.stack.push_back(frame.local_vars[idx]);
  frame.pc += 2;
}

void exec_iload_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(frame.local_vars[0]);
  frame.pc++;
}
void exec_iload_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(frame.local_vars[1]);
  frame.pc++;
}
void exec_iload_2(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(frame.local_vars[2]);
  frame.pc++;
}
void exec_iload_3(Thread *t, Frame &frame) {
  frame.operand_stack.push_int(frame.local_vars[3]);
  frame.pc++;
}
void exec_lload_0(Thread *t, Frame &frame) {
  frame.operand_stack.push_long(
      (static_cast<int64_t>(frame.local_vars[0]) << 32) |
      static_cast<int64_t>(frame.local_vars[1]));
  frame.pc++;
}
void exec_lload_1(Thread *t, Frame &frame) {
  frame.operand_stack.push_long(
      (static_cast<int64_t>(frame.local_vars[1]) << 32) |
      static_cast<int64_t>(frame.local_vars[2]));
  frame.pc++;
}
void exec_lload_2(Thread *t, Frame &frame) {
  frame.operand_stack.push_long(
      (static_cast<int64_t>(frame.local_vars[2]) << 32) |
      static_cast<int64_t>(frame.local_vars[3]));
  frame.pc++;
}
void exec_lload_3(Thread *t, Frame &frame) {
  frame.operand_stack.push_long(
      (static_cast<int64_t>(frame.local_vars[3]) << 32));
  frame.pc++;
}
void exec_aload_0(Thread *t, Frame &frame) {
  frame.operand_stack.stack.push_back(frame.local_vars[0]);
  frame.pc++;
}
void exec_aload_1(Thread *t, Frame &frame) {
  frame.operand_stack.stack.push_back(frame.local_vars[1]);
  frame.pc++;
}
void exec_aload_2(Thread *t, Frame &frame) {
  frame.operand_stack.stack.push_back(frame.local_vars[2]);
  frame.pc++;
}
void exec_aload_3(Thread *t, Frame &frame) {
  frame.operand_stack.stack.push_back(frame.local_vars[3]);
  frame.pc++;
}

void exec_iaload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("iaload: null array reference");
  int32_t value;
  std::memcpy(&value, &array_ref->data[index * sizeof(int32_t)],
              sizeof(int32_t));
  frame.operand_stack.push_int(value);
  frame.pc++;
}

void exec_laload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("laload: null array reference");
  int64_t value = 0;
  std::memcpy(&value, &array_ref->data[index * sizeof(int64_t)],
              sizeof(int64_t));
  frame.operand_stack.push_long(value);
  frame.pc++;
}

void exec_faload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("faload: null array reference");
  float value = 0.0f;
  std::memcpy(&value, &array_ref->data[index * sizeof(float)], sizeof(float));
  frame.operand_stack.push_float(value);
  frame.pc++;
}

void exec_daload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("daload: null array reference");
  double value = 0.0;
  std::memcpy(&value, &array_ref->data[index * sizeof(double)], sizeof(double));
  frame.operand_stack.push_double(value);
  frame.pc++;
}

void exec_aaload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("aaload: null array reference");
  RuntimeObject *value = nullptr;
  std::memcpy(&value, &array_ref->data[index * sizeof(RuntimeObject *)],
              sizeof(RuntimeObject *));
  frame.operand_stack.push_ref(value);
  frame.pc++;
}

void exec_baload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("baload: null array reference");
  uint8_t value = 0;
  std::memcpy(&value, &array_ref->data[index * sizeof(uint8_t)],
              sizeof(uint8_t));
  frame.operand_stack.push_int(value);
  frame.pc++;
}

void exec_caload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("caload: null array reference");
  uint16_t value = 0;
  std::memcpy(&value, &array_ref->data[index * sizeof(uint16_t)],
              sizeof(uint16_t));
  frame.operand_stack.push_int(value); // char também promovido para int
  frame.pc++;
}

void exec_saload(Thread *t, Frame &frame) {
  int32_t index = frame.operand_stack.pop_int();
  RuntimeObject *array_ref = frame.operand_stack.pop_ref();
  if (!array_ref)
    throw std::runtime_error("saload: null array reference");
  int16_t value = 0;
  std::memcpy(&value, &array_ref->data[index * sizeof(int16_t)],
              sizeof(int16_t));
  frame.operand_stack.push_int(value); // short promovido para int
  frame.pc++;
}

// STORES
void exec_istore(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 idx = code[frame.pc + 1];
  frame.local_vars[idx] = frame.operand_stack.pop_int();
  frame.pc += 2;
}

void exec_lstore(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 idx = code[frame.pc + 1];
  int64_t v = frame.operand_stack.pop_long();
  frame.local_vars[idx] = static_cast<u4>((v >> 32) & 0xFFFFFFFF);
  if (idx + 1 < frame.local_vars.size()) {
    frame.local_vars[idx + 1] = static_cast<u4>(v & 0xFFFFFFFF);
  }
  frame.pc += 2;
}

void exec_fstore(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 index = frame.method->code->code[frame.pc + 1];
  float value = frame.operand_stack.pop_float();
  u4 bits;
  std::memcpy(&bits, &value, sizeof(float));
  frame.local_vars[index] = bits;
  frame.pc += 2;
}

void exec_dstore(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 index = frame.method->code->code[frame.pc + 1];
  double value = frame.operand_stack.pop_double();
  uint64_t bits;
  std::memcpy(&bits, &value, sizeof(double));
  frame.local_vars[index] = static_cast<u4>((bits >> 32) & 0xFFFFFFFF);
  if (index + 1 < frame.local_vars.size()) {
    frame.local_vars[index + 1] = static_cast<u4>(bits & 0xFFFFFFFF);
  }
  frame.pc += 2;
}

void exec_astore(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 idx = code[frame.pc + 1];
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  frame.local_vars[idx] = (Slot)(uintptr_t)ref;
  frame.pc += 2;
}

void exec_istore_0(Thread *t, Frame &frame) {
  frame.local_vars[0] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_istore_1(Thread *t, Frame &frame) {
  frame.local_vars[1] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_istore_2(Thread *t, Frame &frame) {
  frame.local_vars[2] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_istore_3(Thread *t, Frame &frame) {
  frame.local_vars[3] = frame.operand_stack.pop_int();
  frame.pc++;
}

void exec_lstore_0(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.local_vars[0] = static_cast<u4>((v >> 32) & 0xFFFFFFFF);
  if (frame.local_vars.size() > 1)
    frame.local_vars[1] = static_cast<u4>(v & 0xFFFFFFFF);
  frame.pc++;
}

void exec_lstore_1(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.local_vars[1] = static_cast<u4>((v >> 32) & 0xFFFFFFFF);
  if (frame.local_vars.size() > 2)
    frame.local_vars[2] = static_cast<u4>(v & 0xFFFFFFFF);
  frame.pc++;
}

void exec_lstore_2(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.local_vars[2] = static_cast<u4>((v >> 32) & 0xFFFFFFFF);
  if (frame.local_vars.size() > 3)
    frame.local_vars[3] = static_cast<u4>(v & 0xFFFFFFFF);
  frame.pc++;
}

void exec_lstore_3(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.local_vars[3] = static_cast<u4>((v >> 32) & 0xFFFFFFFF);
  frame.pc++;
}

void exec_fstore_0(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  u4 bits;
  std::memcpy(&bits, &v, sizeof(float));
  frame.local_vars[0] = bits;
  frame.pc++;
}

void exec_fstore_1(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  u4 bits;
  std::memcpy(&bits, &v, sizeof(float));
  frame.local_vars[1] = bits;
  frame.pc++;
}

void exec_fstore_2(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  u4 bits;
  std::memcpy(&bits, &v, sizeof(float));
  frame.local_vars[2] = bits;
  frame.pc++;
}

void exec_fstore_3(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  u4 bits;
  std::memcpy(&bits, &v, sizeof(float));
  frame.local_vars[3] = bits;
  frame.pc++;
}

void exec_dstore_0(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  uint64_t bits;
  std::memcpy(&bits, &v, sizeof(double));
  frame.local_vars[0] = static_cast<u4>((bits >> 32) & 0xFFFFFFFF);
  if (frame.local_vars.size() > 1)
    frame.local_vars[1] = static_cast<u4>(bits & 0xFFFFFFFF);
  frame.pc++;
}

void exec_dstore_1(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  uint64_t bits;
  std::memcpy(&bits, &v, sizeof(double));
  frame.local_vars[1] = static_cast<u4>((bits >> 32) & 0xFFFFFFFF);
  if (frame.local_vars.size() > 2)
    frame.local_vars[2] = static_cast<u4>(bits & 0xFFFFFFFF);
  frame.pc++;
}

void exec_dstore_2(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  uint64_t bits;
  std::memcpy(&bits, &v, sizeof(double));
  frame.local_vars[2] = static_cast<u4>((bits >> 32) & 0xFFFFFFFF);
  if (frame.local_vars.size() > 3)
    frame.local_vars[3] = static_cast<u4>(bits & 0xFFFFFFFF);
  frame.pc++;
}

void exec_dstore_3(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  uint64_t bits;
  std::memcpy(&bits, &v, sizeof(double));
  frame.local_vars[3] = static_cast<u4>((bits >> 32) & 0xFFFFFFFF);
  frame.pc++;
}

void exec_astore_0(Thread *t, Frame &frame) {
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  frame.local_vars[0] = (Slot)(uintptr_t)ref;
  frame.pc++;
}

void exec_astore_1(Thread *t, Frame &frame) {
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  frame.local_vars[1] = (Slot)(uintptr_t)ref;
  frame.pc++;
}
void exec_astore_2(Thread *t, Frame &frame) {
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  frame.local_vars[2] = (Slot)(uintptr_t)ref;
  frame.pc++;
}
void exec_astore_3(Thread *t, Frame &frame) {
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  frame.local_vars[3] = (Slot)(uintptr_t)ref;
  frame.pc++;
}
void exec_iastore(Thread *t, Frame &frame) {}
void exec_lastore(Thread *t, Frame &frame) {}
void exec_fastore(Thread *t, Frame &frame) {}
void exec_dastore(Thread *t, Frame &frame) {}
void exec_aastore(Thread *t, Frame &frame) {}
void exec_bastore(Thread *t, Frame &frame) {}
void exec_castore(Thread *t, Frame &frame) {}
void exec_sastore(Thread *t, Frame &frame) {}

// STACK OPS
void exec_pop(Thread *t, Frame &frame) {
  frame.operand_stack.stack.pop_back();
  frame.pc++;
}
void exec_pop2(Thread *t, Frame &frame) {
  frame.operand_stack.stack.pop_back();
  frame.operand_stack.stack.pop_back();
  frame.pc++;
}

void exec_dup(Thread *t, Frame &frame) {
  Slot v = frame.operand_stack.stack.back();
  frame.operand_stack.stack.push_back(v);
  frame.pc++;
}

void exec_dup_x1(Thread *t, Frame &frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;
}
void exec_dup_x2(Thread *t, Frame &frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  Slot v3 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v3);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;
}
void exec_dup2(Thread *t, Frame &frame) {
  Slot v1 = frame.operand_stack.pop_int();
  Slot v2 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v2);
  frame.operand_stack.push_int(v1);
  frame.pc++;
}
void exec_dup2_x1(Thread *t, Frame &frame) {
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
void exec_dup2_x2(Thread *t, Frame &frame) {
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

void exec_swap(Thread *t, Frame &frame) {
  u4 v1 = frame.operand_stack.pop_int();
  u4 v2 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1);
  frame.operand_stack.push_int(v2);
  frame.pc += 1;
}

void exec_iadd(Thread *t, Frame &frame) {
  int32_t b = frame.operand_stack.pop_int();
  int32_t a = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(a + b);
  frame.pc++;
}

void exec_ladd(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(v1 + v2);
  frame.pc++;
}

void exec_fadd(Thread *t, Frame &frame) {
  float v2 = frame.operand_stack.pop_float();
  float v1 = frame.operand_stack.pop_float();
  frame.operand_stack.push_float(v1 + v2);
  frame.pc++;
}

void exec_dadd(Thread *t, Frame &frame) {
  double v2 = frame.operand_stack.pop_double();
  double v1 = frame.operand_stack.pop_double();
  frame.operand_stack.push_double(v1 + v2);
  frame.pc++;
}

void exec_isub(Thread *t, Frame &frame) {
  int32_t b = frame.operand_stack.pop_int();
  int32_t a = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(a - b);
  frame.pc++;
}

void exec_lsub(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(v1 - v2);
  frame.pc++;
}

void exec_fsub(Thread *t, Frame &frame) {
  float v2 = frame.operand_stack.pop_float();
  float v1 = frame.operand_stack.pop_float();
  frame.operand_stack.push_float(v1 - v2);
  frame.pc++;
}

void exec_dsub(Thread *t, Frame &frame) {
  double v2 = frame.operand_stack.pop_double();
  double v1 = frame.operand_stack.pop_double();
  frame.operand_stack.push_double(v1 - v2);
  frame.pc++;
}

void exec_imul(Thread *t, Frame &frame) {
  int32_t b = frame.operand_stack.pop_int();
  int32_t a = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(a * b);
  frame.pc++;
}

void exec_lmul(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(v1 * v2);
  frame.pc++;
}

void exec_fmul(Thread *t, Frame &frame) {
  float v2 = frame.operand_stack.pop_float();
  float v1 = frame.operand_stack.pop_float();
  frame.operand_stack.push_float(v1 * v2);
  frame.pc++;
}

void exec_dmul(Thread *t, Frame &frame) {
  double v2 = frame.operand_stack.pop_double();
  double v1 = frame.operand_stack.pop_double();
  frame.operand_stack.push_double(v1 * v2);
  frame.pc++;
}

void exec_idiv(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  if (v2 == 0)
    throw std::runtime_error("ArithmeticException: / by zero");
  frame.operand_stack.push_int(v1 / v2);
  frame.pc++;
}

void exec_ldiv(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  if (v2 == 0)
    throw std::runtime_error("ArithmeticException: / by zero");
  frame.operand_stack.push_long(v1 / v2);
  frame.pc++;
}

void exec_fdiv(Thread *t, Frame &frame) {
  float v2 = frame.operand_stack.pop_float();
  float v1 = frame.operand_stack.pop_float();
  frame.operand_stack.push_float(v1 / v2);
  frame.pc++;
}

void exec_ddiv(Thread *t, Frame &frame) {
  double v2 = frame.operand_stack.pop_double();
  double v1 = frame.operand_stack.pop_double();
  frame.operand_stack.push_double(v1 / v2);
  frame.pc++;
}

void exec_irem(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  if (v2 == 0)
    throw std::runtime_error("ArithmeticException: / by zero");
  frame.operand_stack.push_int(v1 % v2);
  frame.pc++;
}

void exec_lrem(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  if (v2 == 0)
    throw std::runtime_error("ArithmeticException: / by zero");
  frame.operand_stack.push_long(v1 % v2);
  frame.pc++;
}

void exec_frem(Thread *t, Frame &frame) {
  float v2 = frame.operand_stack.pop_float();
  float v1 = frame.operand_stack.pop_float();
  frame.operand_stack.push_float(std::fmod(v1, v2));
  frame.pc++;
}

void exec_drem(Thread *t, Frame &frame) {
  double v2 = frame.operand_stack.pop_double();
  double v1 = frame.operand_stack.pop_double();
  frame.operand_stack.push_double(std::fmod(v1, v2));
  frame.pc++;
}

void exec_ineg(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(-v);
  frame.pc++;
}

void exec_lneg(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(-v);
  frame.pc++;
}

void exec_fneg(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  frame.operand_stack.push_float(-v);
  frame.pc++;
}

void exec_dneg(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  frame.operand_stack.push_double(-v);
  frame.pc++;
}

void exec_ishl(Thread *t, Frame &frame) {
  int32_t shift = frame.operand_stack.pop_int() & 0x1F;
  int32_t value = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(value << shift);
  frame.pc++;
}

void exec_lshl(Thread *t, Frame &frame) {
  int32_t shift = frame.operand_stack.pop_int() & 0x3F;
  int64_t value = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(value << shift);
  frame.pc++;
}

void exec_ishr(Thread *t, Frame &frame) {
  int32_t shift = frame.operand_stack.pop_int() & 0x1F;
  int32_t value = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(value >> shift);
  frame.pc++;
}

void exec_lshr(Thread *t, Frame &frame) {
  int32_t shift = frame.operand_stack.pop_int() & 0x3F;
  int64_t value = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(value >> shift);
  frame.pc++;
}

void exec_iushr(Thread *t, Frame &frame) {
  int32_t shift = frame.operand_stack.pop_int() & 0x1F;
  uint32_t value = static_cast<uint32_t>(frame.operand_stack.pop_int());
  frame.operand_stack.push_int(static_cast<int32_t>(value >> shift));
  frame.pc++;
}

void exec_lushr(Thread *t, Frame &frame) {
  int32_t shift = frame.operand_stack.pop_int() & 0x3F;
  uint64_t value = static_cast<uint64_t>(frame.operand_stack.pop_long());
  frame.operand_stack.push_long(static_cast<int64_t>(value >> shift));
  frame.pc++;
}

void exec_iand(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1 & v2);
  frame.pc++;
}

void exec_land(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(v1 & v2);
  frame.pc++;
}

void exec_ior(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1 | v2);
  frame.pc++;
}

void exec_lor(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(v1 | v2);
  frame.pc++;
}

void exec_ixor(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  frame.operand_stack.push_int(v1 ^ v2);
  frame.pc++;
}

void exec_lxor(Thread *t, Frame &frame) {
  int64_t v2 = frame.operand_stack.pop_long();
  int64_t v1 = frame.operand_stack.pop_long();
  frame.operand_stack.push_long(v1 ^ v2);
  frame.pc++;
}

void exec_iinc(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u1 index = code[frame.pc + 1];
  int8_t constant = static_cast<int8_t>(code[frame.pc + 2]);

  int32_t value = static_cast<int32_t>(frame.local_vars[index]);
  value += constant;
  frame.local_vars[index] = static_cast<u4>(value);

  frame.pc += 3;
}

void exec_i2l(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  frame.operand_stack.push_long(static_cast<int64_t>(v));
  frame.pc++;
}

void exec_i2f(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  frame.operand_stack.push_float(static_cast<float>(v));
  frame.pc++;
}

void exec_i2d(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  frame.operand_stack.push_double(static_cast<double>(v));
  frame.pc++;
}

void exec_l2i(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.operand_stack.push_int(static_cast<int32_t>(v));
  frame.pc++;
}

void exec_l2f(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.operand_stack.push_float(static_cast<float>(v));
  frame.pc++;
}

void exec_l2d(Thread *t, Frame &frame) {
  int64_t v = frame.operand_stack.pop_long();
  frame.operand_stack.push_double(static_cast<double>(v));
  frame.pc++;
}

void exec_f2i(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  frame.operand_stack.push_int(static_cast<int32_t>(v));
  frame.pc++;
}

void exec_f2l(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  frame.operand_stack.push_long(static_cast<int64_t>(v));
  frame.pc++;
}

void exec_f2d(Thread *t, Frame &frame) {
  float v = frame.operand_stack.pop_float();
  frame.operand_stack.push_double(static_cast<double>(v));
  frame.pc++;
}

void exec_d2i(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  frame.operand_stack.push_int(static_cast<int32_t>(v));
  frame.pc++;
}

void exec_d2l(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  frame.operand_stack.push_long(static_cast<int64_t>(v));
  frame.pc++;
}

void exec_d2f(Thread *t, Frame &frame) {
  double v = frame.operand_stack.pop_double();
  frame.operand_stack.push_float((float)v);
  frame.pc += 1;
}

void exec_i2b(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int8_t r = (int8_t)v;
  frame.operand_stack.push_int((int32_t)r);
  frame.pc += 1;
}

void exec_i2c(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  uint16_t r = (uint16_t)v;
  frame.operand_stack.push_int((int32_t)r);
  frame.pc += 1;
}

void exec_i2s(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t r = (int16_t)v;
  frame.operand_stack.push_int((int32_t)r);
  frame.pc += 1;
}

void exec_lcmp(Thread *t, Frame &frame) {
  int64_t b = frame.operand_stack.pop_long();
  int64_t a = frame.operand_stack.pop_long();
  int32_t r = (a == b ? 0 : (a > b ? 1 : -1));
  frame.operand_stack.push_int(r);
  frame.pc += 1;
}

void exec_fcmpl(Thread *t, Frame &frame) {
  float b = frame.operand_stack.pop_float();
  float a = frame.operand_stack.pop_float();
  int32_t r;

  if (std::isnan(a) || std::isnan(b))
    r = -1;
  else
    r = (a == b ? 0 : (a > b ? 1 : -1));

  frame.operand_stack.push_int(r);
  frame.pc += 1;
}

void exec_fcmpg(Thread *t, Frame &frame) {
  float b = frame.operand_stack.pop_float();
  float a = frame.operand_stack.pop_float();
  int32_t r;
  if (std::isnan(a) || std::isnan(b))
    r = 1;
  else
    r = (a == b ? 0 : (a > b ? 1 : -1));
  frame.operand_stack.push_int(r);
  frame.pc += 1;
}

void exec_dcmpl(Thread *t, Frame &frame) {
  double b = frame.operand_stack.pop_double();
  double a = frame.operand_stack.pop_double();
  int32_t r;
  if (std::isnan(a) || std::isnan(b))
    r = -1;
  else
    r = (a == b ? 0 : (a > b ? 1 : -1));
  frame.operand_stack.push_int(r);
  frame.pc += 1;
}

void exec_dcmpg(Thread *t, Frame &frame) {
  double b = frame.operand_stack.pop_double();
  double a = frame.operand_stack.pop_double();
  int32_t r;

  if (std::isnan(a) || std::isnan(b))
    r = 1;
  else
    r = (a == b ? 0 : (a > b ? 1 : -1));

  frame.operand_stack.push_int(r);
  frame.pc += 1;
}

static inline int16_t read_s16(Frame &frame) {
  u1 b1 = frame.method->code->code[frame.pc + 1];
  u1 b2 = frame.method->code->code[frame.pc + 2];
  return (int16_t)((b1 << 8) | b2);
}

void exec_ifeq(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v == 0 ? off : 3);
}

void exec_ifne(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v != 0 ? off : 3);
}

void exec_iflt(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v < 0 ? off : 3);
}

void exec_ifge(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v >= 0 ? off : 3);
}

void exec_ifgt(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v > 0 ? off : 3);
}

void exec_ifle(Thread *t, Frame &frame) {
  int32_t v = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v <= 0 ? off : 3);
}

void exec_if_icmpeq(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v1 == v2 ? off : 3);
}

void exec_if_icmpne(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v1 != v2 ? off : 3);
}

void exec_if_icmplt(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v1 < v2 ? off : 3);
}

void exec_if_icmpge(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v1 >= v2 ? off : 3);
}

void exec_if_icmpgt(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v1 > v2 ? off : 3);
}

void exec_if_icmple(Thread *t, Frame &frame) {
  int32_t v2 = frame.operand_stack.pop_int();
  int32_t v1 = frame.operand_stack.pop_int();
  int16_t off = read_s16(frame);
  frame.pc += (v1 <= v2 ? off : 3);
}

void exec_if_acmpeq(Thread *t, Frame &frame) {
  u4 ref2 = (u4)frame.operand_stack.pop_ref();
  u4 ref1 = (u4)frame.operand_stack.pop_ref();
  int16_t off = read_s16(frame);
  frame.pc += (ref1 == ref2 ? off : 3);
}

void exec_if_acmpne(Thread *t, Frame &frame) {
  u4 ref2 = (u4)frame.operand_stack.pop_ref();
  u4 ref1 = (u4)frame.operand_stack.pop_ref();
  int16_t off = read_s16(frame);
  frame.pc += (ref1 != ref2 ? off : 3);
}

void exec_goto(Thread *t, Frame &frame) {
  int16_t off = read_s16(frame);
  frame.pc += off; // sempre pula
}

// JSR empilha o endereço de retorno (pc + 3)
// e então salta para offset
void exec_jsr(Thread *t, Frame &frame) {
  int16_t off = read_s16(frame);
  int32_t return_addr = frame.pc + 3;
  frame.operand_stack.push_int(return_addr);
  frame.pc += off;
}

void exec_ret(Thread *t, Frame &frame) {
  u1 index = frame.method->code->code[frame.pc + 1];
  int32_t addr = frame.local_vars[index];
  frame.pc = addr;
}

void exec_tableswitch(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();

  // Alinhar para múltiplo de 4 após o opcode
  u4 base = frame.pc + 1;
  while (base % 4 != 0)
    base++;

  // Lê default, low e high (todos u4 big-endian)
  auto read_u4 = [&](u4 pos) {
    return (code[pos] << 24) | (code[pos + 1] << 16) | (code[pos + 2] << 8) |
           code[pos + 3];
  };

  int32_t key = frame.operand_stack.pop_int();

  u4 default_offset = read_u4(base);
  int32_t low = static_cast<int32_t>(read_u4(base + 4));
  int32_t high = static_cast<int32_t>(read_u4(base + 8));

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

void exec_lookupswitch(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();

  u4 base = frame.pc + 1;
  while (base % 4 != 0)
    base++;

  auto read_u4 = [&](u4 pos) {
    return (code[pos] << 24) | (code[pos + 1] << 16) | (code[pos + 2] << 8) |
           code[pos + 3];
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

void exec_ireturn(Thread *thread, Frame &frame) {
  int32_t value = frame.operand_stack.pop_int();
  thread->pop_frame();
  Frame &caller = thread->current_frame();
  caller.operand_stack.push_int(value);
}

void exec_lreturn(Thread *thread, Frame &frame) {
  int64_t value = frame.operand_stack.pop_long();
  thread->pop_frame();
  Frame &caller = thread->current_frame();
  caller.operand_stack.push_long(value);
}

void exec_freturn(Thread *thread, Frame &frame) {
  float value = frame.operand_stack.pop_float();
  thread->pop_frame();
  Frame &caller = thread->current_frame();
  caller.operand_stack.push_float(value);
}

void exec_dreturn(Thread *thread, Frame &frame) {
  double value = frame.operand_stack.pop_double();
  thread->pop_frame();
  Frame &caller = thread->current_frame();
  caller.operand_stack.push_double(value);
}

void exec_areturn(Thread *thread, Frame &frame) {
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  thread->pop_frame();
  Frame &caller = thread->current_frame();
  caller.operand_stack.push_ref(ref);
}

void exec_return(Thread *thread, Frame &) { thread->pop_frame(); }

static RuntimeField *resolve_field(RuntimeClass *current_class, u2 index,
                                   Thread *thread) {
  if (!current_class || !current_class->class_file)
    throw std::runtime_error("resolve_field: missing class file");
  auto &cp = current_class->class_file->constant_pool;
  if (index == 0 || index >= cp.size() ||
      cp[index].first != ConstantTag::CONSTANT_Fieldref)
    throw std::runtime_error("resolve_field: invalid cp index");

  const auto &fieldref = cp[index].second.fieldref_info;
  u2 nat_idx = fieldref.name_and_type_index;
  if (nat_idx == 0 || nat_idx >= cp.size() ||
      cp[nat_idx].first != ConstantTag::CONSTANT_NameAndType)
    throw std::runtime_error("resolve_field: invalid name_and_type");

  std::string name = current_class->class_file->resolve_utf8(
      cp[nat_idx].second.name_and_type_info.name_index);
  std::string desc = current_class->class_file->resolve_utf8(
      cp[nat_idx].second.name_and_type_info.descriptor_index);
  std::string key = desc + " " + name;

  // try current class first
  auto it_cur = current_class->fields.find(key);
  if (it_cur != current_class->fields.end())
    return &it_cur->second;

  // try to load referenced class
  std::string class_name = current_class->class_file->resolve_utf8(
      cp[fieldref.class_index].second.class_info.name_index);
  RuntimeClass *target = nullptr;
  if (thread && thread->runtime) {
    if (auto loaded = thread->runtime->method_area->getClassRef(class_name)) {
      target = loaded;
    } else {
      target = thread->runtime->class_loader->load_class(class_name);
    }
  }
  if (target) {
    auto it_tgt = target->fields.find(key);
    if (it_tgt != target->fields.end())
      return &it_tgt->second;
  }
  throw std::runtime_error("resolve_field: field not found");
}

static RuntimeClass *resolve_class_cp(RuntimeClass *current_class, u2 index,
                                      Thread *thread) {
  if (!current_class || !current_class->class_file)
    throw std::runtime_error("resolve_class: missing class file");
  auto &cp = current_class->class_file->constant_pool;
  if (index == 0 || index >= cp.size() ||
      cp[index].first != ConstantTag::CONSTANT_Class)
    throw std::runtime_error("resolve_class: invalid cp index");

  u2 name_idx = cp[index].second.class_info.name_index;
  std::string name = current_class->class_file->resolve_utf8(name_idx);

  RuntimeClass *target = nullptr;
  if (thread && thread->runtime) {
    if (auto loaded = thread->runtime->method_area->getClassRef(name)) {
      target = loaded;
    } else {
      target = thread->runtime->class_loader->load_class(name);
    }
  }
  if (!target)
    throw std::runtime_error("resolve_class: unable to load " + name);
  return target;
}

static RuntimeMethod *resolve_method_special(RuntimeClass *current_class,
                                             u2 index, Thread *thread) {
  if (!current_class || !current_class->class_file)
    throw std::runtime_error("resolve_method: missing class file");
  auto &cp = current_class->class_file->constant_pool;
  if (index == 0 || index >= cp.size())
    throw std::runtime_error("resolve_method: invalid cp index");

  const auto &entry = cp[index];
  if (entry.first != ConstantTag::CONSTANT_Methodref &&
      entry.first != ConstantTag::CONSTANT_InterfaceMethodref)
    throw std::runtime_error("resolve_method: cp entry not a methodref");

  u2 class_index = (entry.first == ConstantTag::CONSTANT_Methodref)
                       ? entry.second.methodref_info.class_index
                       : entry.second.interface_methodref_info.class_index;
  u2 nat_idx = (entry.first == ConstantTag::CONSTANT_Methodref)
                   ? entry.second.methodref_info.name_and_type_index
                   : entry.second.interface_methodref_info.name_and_type_index;

  std::string class_name =
      current_class->class_file->resolve_utf8(
          cp[class_index].second.class_info.name_index);
  std::string name = current_class->class_file->resolve_utf8(
      cp[nat_idx].second.name_and_type_info.name_index);
  std::string desc = current_class->class_file->resolve_utf8(
      cp[nat_idx].second.name_and_type_info.descriptor_index);
  std::string key = desc + " " + name;

  RuntimeClass *target = nullptr;
  if (thread && thread->runtime) {
    if (auto loaded = thread->runtime->method_area->getClassRef(class_name)) {
      target = loaded;
    } else {
      target = thread->runtime->class_loader->load_class(class_name);
    }
  }
  if (!target)
    throw std::runtime_error("resolve_method: class not found " + class_name);

  auto it = target->methods.find(key);
  if (it == target->methods.end())
    throw std::runtime_error("resolve_method: method not found " + key);

  return &it->second;
}

static RuntimeMethod *resolve_method_static(RuntimeClass *current_class,
                                            u2 index, Thread *thread) {
  if (!current_class || !current_class->class_file)
    throw std::runtime_error("resolve_method: missing class file");
  auto &cp = current_class->class_file->constant_pool;
  if (index == 0 || index >= cp.size() ||
      cp[index].first != ConstantTag::CONSTANT_Methodref)
    throw std::runtime_error("resolve_method: invalid cp index");

  u2 class_index = cp[index].second.methodref_info.class_index;
  u2 nat_idx = cp[index].second.methodref_info.name_and_type_index;

  std::string class_name =
      current_class->class_file->resolve_utf8(
          cp[class_index].second.class_info.name_index);
  std::string name = current_class->class_file->resolve_utf8(
      cp[nat_idx].second.name_and_type_info.name_index);
  std::string desc = current_class->class_file->resolve_utf8(
      cp[nat_idx].second.name_and_type_info.descriptor_index);
  std::string key = desc + " " + name;

  RuntimeClass *target = nullptr;
  if (thread && thread->runtime) {
    if (auto loaded = thread->runtime->method_area->getClassRef(class_name)) {
      target = loaded;
    } else {
      target = thread->runtime->class_loader->load_class(class_name);
    }
  }
  if (!target)
    throw std::runtime_error("resolve_method: class not found " + class_name);

  auto it = target->methods.find(key);
  if (it == target->methods.end())
    throw std::runtime_error("resolve_method: method not found " + key);

  return &it->second;
}

void exec_getstatic(Thread *t, Frame &frame) {
  u1 op1 = frame.method->code->code[frame.pc + 1];
  u1 op2 = frame.method->code->code[frame.pc + 2];
  u2 index = (op1 << 8) | op2;

  RuntimeField *field = resolve_field(frame.current_class, index, t);
  if (!field->is_static)
    throw std::runtime_error("getstatic on non-static field");

  if (field->is_64bit) {
    int64_t v = 0;
    std::memcpy(&v, &field->owner->static_data[field->static_offset],
                sizeof(int64_t));
    frame.operand_stack.push_long(v);
  } else {
    u4 v = 0;
    std::memcpy(&v, &field->owner->static_data[field->static_offset],
                sizeof(u4));
    frame.operand_stack.push_int(static_cast<int32_t>(v));
  }

  frame.pc += 3;
}

void exec_putstatic(Thread *t, Frame &frame) {
  u1 op1 = frame.method->code->code[frame.pc + 1];
  u1 op2 = frame.method->code->code[frame.pc + 2];
  u2 index = (op1 << 8) | op2;

  RuntimeField *field = resolve_field(frame.current_class, index, t);
  if (!field->is_static)
    throw std::runtime_error("putstatic on non-static field");

  if (field->is_64bit) {
    int64_t v = frame.operand_stack.pop_long();
    std::memcpy(&field->owner->static_data[field->static_offset], &v,
                sizeof(int64_t));
  } else {
    u4 v = frame.operand_stack.pop_int();
    std::memcpy(&field->owner->static_data[field->static_offset], &v,
                sizeof(u4));
  }

  frame.pc += 3;
}

void exec_getfield(Thread *t, Frame &frame) {
  u2 index = (frame.method->code->code[frame.pc + 1] << 8) |
             frame.method->code->code[frame.pc + 2];

  RuntimeField *field = resolve_field(frame.current_class, index, t);
  if (field->is_static)
    throw std::runtime_error("getfield on static field");

  RuntimeObject *obj = frame.operand_stack.pop_ref();
  if (!obj)
    throw std::runtime_error("getfield: null pointer");

  if (field->is_64bit) {
    int64_t v = obj->read_field<int64_t>(*field);
    frame.operand_stack.push_long(v);
  } else {
    u4 v = obj->read_field<u4>(*field);
    frame.operand_stack.push_int(static_cast<int32_t>(v));
  }

  frame.pc += 3;
}

void exec_putfield(Thread *t, Frame &frame) {
  u2 index = (frame.method->code->code[frame.pc + 1] << 8) |
             frame.method->code->code[frame.pc + 2];

  RuntimeField *field = resolve_field(frame.current_class, index, t);
  if (field->is_static)
    throw std::runtime_error("putfield on static field");

  if (field->is_64bit) {
    int64_t v = frame.operand_stack.pop_long();
    RuntimeObject *obj = frame.operand_stack.pop_ref();
    if (!obj)
      throw std::runtime_error("putfield: null ptr");
    obj->write_field<int64_t>(*field, v);
  } else {
    u4 v = frame.operand_stack.pop_int();
    RuntimeObject *obj = frame.operand_stack.pop_ref();
    if (!obj)
      throw std::runtime_error("putfield: null ptr");
    obj->write_field<u4>(*field, v);
  }

  frame.pc += 3;
}

Frame *invoke_method(RuntimeMethod *method, std::vector<Slot> args) {
  (void)method;
  (void)args;
  throw std::runtime_error("invoke_method helper not implemented");
}

void exec_invokespecial(Thread *t, Frame &frame) {
  u2 index = (frame.method->code->code[frame.pc + 1] << 8) |
             frame.method->code->code[frame.pc + 2];

  RuntimeMethod *method = resolve_method_special(frame.current_class, index, t);
  if (!method || !method->code)
    throw std::runtime_error("invokespecial: method has no code");

  std::vector<Slot> locals(method->code->max_locals, 0);
  for (int i = method->arg_slots - 1; i >= 0; --i) {
    locals[i] = frame.operand_stack.stack.back();
    frame.operand_stack.stack.pop_back();
  }

  Frame *new_frame = new Frame(method, method->owner);
  new_frame->init(method->code->max_locals, method->code->max_stack);
  new_frame->local_vars.swap(locals);

  t->push_frame(new_frame);

  frame.pc += 3;
}

void exec_invokestatic(Thread *t, Frame &frame) {
  u2 index = (frame.method->code->code[frame.pc + 1] << 8) |
             frame.method->code->code[frame.pc + 2];

  RuntimeMethod *method = resolve_method_static(frame.current_class, index, t);
  if (!method || !method->code)
    throw std::runtime_error("invokestatic: method has no code");

  std::vector<Slot> locals(method->code->max_locals, 0);
  for (int i = method->arg_slots - 1; i >= 0; --i) {
    locals[i] = frame.operand_stack.stack.back();
    frame.operand_stack.stack.pop_back();
  }

  Frame *new_frame = new Frame(method, method->owner);
  new_frame->init(method->code->max_locals, method->code->max_stack);
  new_frame->local_vars.swap(locals);

  t->push_frame(new_frame);

  frame.pc += 3;
}

void exec_invokedynamic(Thread *t, Frame &frame) {
  throw std::runtime_error("invokedynamic: not implemented in this JVM");
}

void exec_invokevirtual(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("invokevirtual not implemented");
}

void exec_invokeinterface(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("invokeinterface not implemented");
}

void exec_new(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u2 index = (code[frame.pc + 1] << 8) | code[frame.pc + 2];

  RuntimeClass *klass = resolve_class_cp(frame.current_class, index, t);
  RuntimeObject *obj = new RuntimeObject(klass);

  frame.operand_stack.push_ref(obj);
  frame.pc += 3;
}

void exec_newarray(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("newarray not implemented");
}

void exec_anewarray(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  u2 index = (code[frame.pc + 1] << 8) | code[frame.pc + 2];
  int32_t count = frame.operand_stack.pop_int();

  if (count < 0)
    throw std::runtime_error("anewarray: negative size");

  RuntimeClass *klass = resolve_class_cp(frame.current_class, index, t);
  RuntimeArray *arr = RuntimeArray::create_reference(klass, static_cast<size_t>(count));

  frame.operand_stack.push_ref(reinterpret_cast<RuntimeObject *>(arr));
  frame.pc += 3;
}

void exec_arraylength(Thread *t, Frame &frame) {
  int ref = frame.operand_stack.pop_int();
  RuntimeArray *arr = (RuntimeArray*)(ref);

  if (!arr)
      throw std::runtime_error("arraylength: null pointer");
  frame.operand_stack.push_int(arr->length);
  frame.pc++;
}

void exec_athrow(Thread *t, Frame &frame) {
  RuntimeObject *exception = frame.operand_stack.pop_ref();
  if (!exception)
    throw std::runtime_error("athrow: null");

  throw_java_exception(exception);

  // nunca volta
}

void exec_checkcast(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("checkcast not implemented");
}

void exec_instanceof(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("instanceof not implemented");
}

void exec_monitorenter(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("monitorenter not implemented");
}

void exec_monitorexit(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("monitorexit not implemented");
}

void exec_wide(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("wide not implemented");
}

void exec_multianewarray(Thread *t, Frame &frame) {
  (void)t;
  (void)frame;
  throw std::runtime_error("multianewarray not implemented");
}

void exec_ifnull(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  int16_t offset = read_s16(frame);
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  // Não verifica caso seja array
  if (ref == nullptr) {
        frame.pc = frame.pc + offset;
        return;
    }
    frame.pc += 3;
}

void exec_ifnonnull(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  int16_t offset = read_s16(frame);
  RuntimeObject *ref = frame.operand_stack.pop_ref();
  // Não verifica caso seja array
  if (ref != nullptr) {
        frame.pc = frame.pc + offset;
        return;
    }
    frame.pc += 3;
}

void exec_goto_w(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  int32_t offset = (code[frame.pc] << 24 | code[frame.pc + 1] << 16 |
  code[frame.pc + 2] | code[frame.pc + 3]);
  frame.pc = frame.pc + offset;
}

void exec_jsr_w(Thread *t, Frame &frame) {
  const u1 *code = frame.method->code->code.data();
  int32_t offset = (code[frame.pc] << 24 | code[frame.pc + 1] << 16 |
  code[frame.pc + 2] | code[frame.pc + 3]);
  u4 return_addr = frame.pc + 5;
  frame.operand_stack.push_int((int32_t)return_addr);
  frame.pc = frame.pc + offset;
}
