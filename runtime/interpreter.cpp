#include "../classfile/class_parser.h"
#include "./runtime_class_types.h"
#include "interpreter.h"

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

        {0x15, exec_iload},
        {0x19, exec_aload},

        {0x1A, exec_iload_0},
        {0x1B, exec_iload_1},
        {0x1C, exec_iload_2},
        {0x1D, exec_iload_3},

        {0x2A, exec_aload_0},
        {0x2B, exec_aload_1},
        {0x2C, exec_aload_2},
        {0x2D, exec_aload_3},

        {0x36, exec_istore},
        {0x3A, exec_astore},

        {0x3B, exec_istore_0},
        {0x4B, exec_astore_0},

        {0x57, exec_pop},
        {0x58, exec_pop2},
        {0x59, exec_dup},

        {0x60, exec_iadd},
        {0x64, exec_isub},
        {0x68, exec_imul},
        {0x6C, exec_idiv},

        {0x99, exec_ifeq},
        {0xA7, exec_goto},

        {0xAC, exec_ireturn},
        {0xB1, exec_return},
    };
}

void exec_aload_0(Frame& frame) {
  Slot value = frame.local_vars[0];
  frame.operand_stack.stack.push_back(value);
  frame.pc += 1;
}

void exec_nop(Frame& frame) {
    frame.pc += 1;
}
void exec_aconst_null(Frame& frame) {
    frame.operand_stack.push_ref(nullptr);
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


// PUSH
void exec_bipush(Frame& frame) {
    int8_t val = (int8_t)frame.method->code->code[frame.pc+1];
    frame.operand_stack.push_int(val);
    frame.pc += 2;
}

void exec_sipush(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    int16_t val = (code[frame.pc+1] << 8) | code[frame.pc+2];
    frame.operand_stack.push_int(val);
    frame.pc += 3;
}


// LOADS
void exec_iload(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    u1 idx = code[frame.pc+1];
    frame.operand_stack.push_int(frame.local_vars[idx]);
    frame.pc += 2;
}

void exec_aload(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    u1 idx = code[frame.pc+1];
    Slot v = frame.local_vars[idx];
    frame.operand_stack.stack.push_back(v);
    frame.pc += 2;
}

void exec_iload_0(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[0]); frame.pc++; }
void exec_iload_1(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[1]); frame.pc++; }
void exec_iload_2(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[2]); frame.pc++; }
void exec_iload_3(Frame& frame) { frame.operand_stack.push_int(frame.local_vars[3]); frame.pc++; }

void exec_aload_0(Frame& frame) { Slot v=frame.local_vars[0]; frame.operand_stack.stack.push_back(v); frame.pc++; }
void exec_aload_1(Frame& frame) { Slot v=frame.local_vars[1]; frame.operand_stack.stack.push_back(v); frame.pc++; }
void exec_aload_2(Frame& frame) { Slot v=frame.local_vars[2]; frame.operand_stack.stack.push_back(v); frame.pc++; }
void exec_aload_3(Frame& frame) { Slot v=frame.local_vars[3]; frame.operand_stack.stack.push_back(v); frame.pc++; }


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

void exec_istore_0(Frame& frame){ frame.local_vars[0] = frame.operand_stack.pop_int(); frame.pc++; }
void exec_astore_0(Frame& frame){
    RuntimeObject* ref = frame.operand_stack.pop_ref();
    frame.local_vars[0] = (Slot)(uintptr_t)ref;
    frame.pc++;
}


// STACK OPS
void exec_pop(Frame& frame)   { frame.operand_stack.stack.pop_back(); frame.pc++; }
void exec_pop2(Frame& frame)  { frame.operand_stack.stack.pop_back(); frame.operand_stack.stack.pop_back(); frame.pc++; }

void exec_dup(Frame& frame) {
    Slot v = frame.operand_stack.stack.back();
    frame.operand_stack.stack.push_back(v);
    frame.pc++;
}


// ARITMÉTICA
void exec_iadd(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a + b);
    frame.pc++;
}

void exec_isub(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a - b);
    frame.pc++;
}

void exec_imul(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a * b);
    frame.pc++;
}

void exec_idiv(Frame& frame) {
    int32_t b = frame.operand_stack.pop_int();
    int32_t a = frame.operand_stack.pop_int();
    frame.operand_stack.push_int(a / b);
    frame.pc++;
}


// COMPARAÇÕES
void exec_ifeq(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    int16_t branch = (code[frame.pc+1] << 8) | code[frame.pc+2];
    int32_t v = frame.operand_stack.pop_int();
    frame.pc += (v == 0 ? branch : 3);
}


// GOTO
void exec_goto(Frame& frame) {
    const u1* code = frame.method->code->code.data();
    int16_t off = (code[frame.pc+1] << 8) | code[frame.pc+2];
    frame.pc += off;
}



