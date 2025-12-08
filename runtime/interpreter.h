#pragma once

#include "./runtime_class_types.h"

/**
 * @file interpreter.h
 * @brief Declaração das funções de execução para cada instrução de bytecode (opcode).
 * Cada função implementa a lógica definida na especificação da JVM.
 */

void exec_nop(Thread *, Frame &);                  ///< 0x00: Não faz nada.
void exec_aconst_null(Thread *, Frame &);          ///< 0x01: Empilha referência null.
void exec_iconst_m1(Thread *, Frame &);            ///< 0x02: Empilha int -1.
void exec_iconst_0(Thread *, Frame &);             ///< 0x03: Empilha int 0.
void exec_iconst_1(Thread *, Frame &);             ///< 0x04: Empilha int 1.
void exec_iconst_2(Thread *, Frame &);             ///< 0x05: Empilha int 2.
void exec_iconst_3(Thread *, Frame &);             ///< 0x06: Empilha int 3.
void exec_iconst_4(Thread *, Frame &);             ///< 0x07: Empilha int 4.
void exec_iconst_5(Thread *, Frame &);             ///< 0x08: Empilha int 5.
void exec_lconst_0(Thread *, Frame &);             ///< 0x09: Empilha long 0.
void exec_lconst_1(Thread *, Frame &);             ///< 0x0A: Empilha long 1.
void exec_fconst_0(Thread *, Frame &);             ///< 0x0B: Empilha float 0.0.
void exec_fconst_1(Thread *, Frame &);             ///< 0x0C: Empilha float 1.0.
void exec_fconst_2(Thread *, Frame &);             ///< 0x0D: Empilha float 2.0.
void exec_dconst_0(Thread *, Frame &);             ///< 0x0E: Empilha double 0.0.
void exec_dconst_1(Thread *, Frame &);             ///< 0x0F: Empilha double 1.0.
void exec_bipush(Thread *, Frame &);               ///< 0x10: Empilha byte (extensão de sinal).
void exec_sipush(Thread *, Frame &);               ///< 0x11: Empilha short (extensão de sinal).
void exec_ldc(Thread *, Frame &);                  ///< 0x12: Empilha item da Constant Pool (índice 8 bits).
void exec_ldc_w(Thread *, Frame &);                ///< 0x13: Empilha item da Constant Pool (índice 16 bits).
void exec_ldc2_w(Thread *, Frame &);               ///< 0x14: Empilha item long/double da Constant Pool.

// Loads (Carregar variáveis locais para a pilha)
void exec_iload(Thread *, Frame &);                ///< 0x15: Carrega int de variável local.
void exec_lload(Thread *, Frame &);                ///< 0x16: Carrega long de variável local.
void exec_fload(Thread *, Frame &);                ///< 0x17: Carrega float de variável local.
void exec_dload(Thread *, Frame &);                ///< 0x18: Carrega double de variável local.
void exec_aload(Thread *, Frame &);                ///< 0x19: Carrega referência de variável local.
void exec_iload_0(Thread *, Frame &);              ///< 0x1A: Carrega int da var local 0.
void exec_iload_1(Thread *, Frame &);              ///< 0x1B: Carrega int da var local 1.
void exec_iload_2(Thread *, Frame &);              ///< 0x1C: Carrega int da var local 2.
void exec_iload_3(Thread *, Frame &);              ///< 0x1D: Carrega int da var local 3.
void exec_lload_0(Thread *, Frame &);              ///< 0x1E: Carrega long da var local 0.
void exec_lload_1(Thread *, Frame &);              ///< 0x1F: Carrega long da var local 1.
void exec_lload_2(Thread *, Frame &);              ///< 0x20: Carrega long da var local 2.
void exec_lload_3(Thread *, Frame &);              ///< 0x21: Carrega long da var local 3.
void exec_fload_0(Thread *, Frame &);              ///< 0x22: Carrega float da var local 0.
void exec_fload_1(Thread *, Frame &);              ///< 0x23: Carrega float da var local 1.
void exec_fload_2(Thread *, Frame &);              ///< 0x24: Carrega float da var local 2.
void exec_fload_3(Thread *, Frame &);              ///< 0x25: Carrega float da var local 3.
void exec_dload_0(Thread *, Frame &);              ///< 0x26: Carrega double da var local 0.
void exec_dload_1(Thread *, Frame &);              ///< 0x27: Carrega double da var local 1.
void exec_dload_2(Thread *, Frame &);              ///< 0x28: Carrega double da var local 2.
void exec_dload_3(Thread *, Frame &);              ///< 0x29: Carrega double da var local 3.
void exec_aload_0(Thread *, Frame &);              ///< 0x2A: Carrega referência da var local 0.
void exec_aload_1(Thread *, Frame &);              ///< 0x2B: Carrega referência da var local 1.
void exec_aload_2(Thread *, Frame &);              ///< 0x2C: Carrega referência da var local 2.
void exec_aload_3(Thread *, Frame &);              ///< 0x2D: Carrega referência da var local 3.
void exec_iaload(Thread *, Frame &);               ///< 0x2E: Carrega int de array.
void exec_laload(Thread *, Frame &);               ///< 0x2F: Carrega long de array.
void exec_faload(Thread *, Frame &);               ///< 0x30: Carrega float de array.
void exec_daload(Thread *, Frame &);               ///< 0x31: Carrega double de array.
void exec_aaload(Thread *, Frame &);               ///< 0x32: Carrega referência de array.
void exec_baload(Thread *, Frame &);               ///< 0x33: Carrega byte/boolean de array.
void exec_caload(Thread *, Frame &);               ///< 0x34: Carrega char de array.
void exec_saload(Thread *, Frame &);               ///< 0x35: Carrega short de array.

// Stores (Salvar da pilha para variáveis locais)
void exec_istore(Thread *, Frame &);               ///< 0x36: Armazena int em variável local.
void exec_lstore(Thread *, Frame &);               ///< 0x37: Armazena long em variável local.
void exec_fstore(Thread *, Frame &);               ///< 0x38: Armazena float em variável local.
void exec_dstore(Thread *, Frame &);               ///< 0x39: Armazena double em variável local.
void exec_astore(Thread *, Frame &);               ///< 0x3A: Armazena referência em variável local.
void exec_istore_0(Thread *, Frame &);             ///< 0x3B: Armazena int na var local 0.
void exec_istore_1(Thread *, Frame &);             ///< 0x3C: Armazena int na var local 1.
void exec_istore_2(Thread *, Frame &);             ///< 0x3D: Armazena int na var local 2.
void exec_istore_3(Thread *, Frame &);             ///< 0x3E: Armazena int na var local 3.
void exec_lstore_0(Thread *, Frame &);             ///< 0x3F: Armazena long na var local 0.
void exec_lstore_1(Thread *, Frame &);             ///< 0x40: Armazena long na var local 1.
void exec_lstore_2(Thread *, Frame &);             ///< 0x41: Armazena long na var local 2.
void exec_lstore_3(Thread *, Frame &);             ///< 0x42: Armazena long na var local 3.
void exec_fstore_0(Thread *, Frame &);             ///< 0x43: Armazena float na var local 0.
void exec_fstore_1(Thread *, Frame &);             ///< 0x44: Armazena float na var local 1.
void exec_fstore_2(Thread *, Frame &);             ///< 0x45: Armazena float na var local 2.
void exec_fstore_3(Thread *, Frame &);             ///< 0x46: Armazena float na var local 3.
void exec_dstore_0(Thread *, Frame &);             ///< 0x47: Armazena double na var local 0.
void exec_dstore_1(Thread *, Frame &);             ///< 0x48: Armazena double na var local 1.
void exec_dstore_2(Thread *, Frame &);             ///< 0x49: Armazena double na var local 2.
void exec_dstore_3(Thread *, Frame &);             ///< 0x4A: Armazena double na var local 3.
void exec_astore_0(Thread *, Frame &);             ///< 0x4B: Armazena referência na var local 0.
void exec_astore_1(Thread *, Frame &);             ///< 0x4C: Armazena referência na var local 1.
void exec_astore_2(Thread *, Frame &);             ///< 0x4D: Armazena referência na var local 2.
void exec_astore_3(Thread *, Frame &);             ///< 0x4E: Armazena referência na var local 3.
void exec_iastore(Thread *, Frame &);              ///< 0x4F: Armazena int em array.
void exec_lastore(Thread *, Frame &);              ///< 0x50: Armazena long em array.
void exec_fastore(Thread *, Frame &);              ///< 0x51: Armazena float em array.
void exec_dastore(Thread *, Frame &);              ///< 0x52: Armazena double em array.
void exec_aastore(Thread *, Frame &);              ///< 0x53: Armazena referência em array.
void exec_bastore(Thread *, Frame &);              ///< 0x54: Armazena byte/boolean em array.
void exec_castore(Thread *, Frame &);              ///< 0x55: Armazena char em array.
void exec_sastore(Thread *, Frame &);              ///< 0x56: Armazena short em array.

// Stack Management
void exec_pop(Thread *, Frame &);                  ///< 0x57: Descarta o valor do topo.
void exec_pop2(Thread *, Frame &);                 ///< 0x58: Descarta os 2 valores do topo.
void exec_dup(Thread *, Frame &);                  ///< 0x59: Duplica o valor do topo.
void exec_dup_x1(Thread *, Frame &);               ///< 0x5A: Duplica topo e insere 2 abaixo.
void exec_dup_x2(Thread *, Frame &);               ///< 0x5B: Duplica topo e insere 3 abaixo.
void exec_dup2(Thread *, Frame &);                 ///< 0x5C: Duplica os 2 valores do topo.
void exec_dup2_x1(Thread *, Frame &);              ///< 0x5D: Dup2 e insere 1 abaixo.
void exec_dup2_x2(Thread *, Frame &);              ///< 0x5E: Dup2 e insere 2 abaixo.
void exec_swap(Thread *, Frame &);                 ///< 0x5F: Troca os dois valores do topo.

// Math
void exec_iadd(Thread *, Frame &);                 ///< 0x60: Soma int.
void exec_ladd(Thread *, Frame &);                 ///< 0x61: Soma long.
void exec_fadd(Thread *, Frame &);                 ///< 0x62: Soma float.
void exec_dadd(Thread *, Frame &);                 ///< 0x63: Soma double.
void exec_isub(Thread *, Frame &);                 ///< 0x64: Subtrai int.
void exec_lsub(Thread *, Frame &);                 ///< 0x65: Subtrai long.
void exec_fsub(Thread *, Frame &);                 ///< 0x66: Subtrai float.
void exec_dsub(Thread *, Frame &);                 ///< 0x67: Subtrai double.
void exec_imul(Thread *, Frame &);                 ///< 0x68: Multiplica int.
void exec_lmul(Thread *, Frame &);                 ///< 0x69: Multiplica long.
void exec_fmul(Thread *, Frame &);                 ///< 0x6A: Multiplica float.
void exec_dmul(Thread *, Frame &);                 ///< 0x6B: Multiplica double.
void exec_idiv(Thread *, Frame &);                 ///< 0x6C: Divide int.
void exec_ldiv(Thread *, Frame &);                 ///< 0x6D: Divide long.
void exec_fdiv(Thread *, Frame &);                 ///< 0x6E: Divide float.
void exec_ddiv(Thread *, Frame &);                 ///< 0x6F: Divide double.
void exec_irem(Thread *, Frame &);                 ///< 0x70: Resto da divisão int.
void exec_lrem(Thread *, Frame &);                 ///< 0x71: Resto da divisão long.
void exec_frem(Thread *, Frame &);                 ///< 0x72: Resto da divisão float.
void exec_drem(Thread *, Frame &);                 ///< 0x73: Resto da divisão double.
void exec_ineg(Thread *, Frame &);                 ///< 0x74: Negação int.
void exec_lneg(Thread *, Frame &);                 ///< 0x75: Negação long.
void exec_fneg(Thread *, Frame &);                 ///< 0x76: Negação float.
void exec_dneg(Thread *, Frame &);                 ///< 0x77: Negação double.
void exec_ishl(Thread *, Frame &);                 ///< 0x78: Shift left int.
void exec_lshl(Thread *, Frame &);                 ///< 0x79: Shift left long.
void exec_ishr(Thread *, Frame &);                 ///< 0x7A: Shift right int (aritmético).
void exec_lshr(Thread *, Frame &);                 ///< 0x7B: Shift right long (aritmético).
void exec_iushr(Thread *, Frame &);                ///< 0x7C: Shift right int (lógico).
void exec_lushr(Thread *, Frame &);                ///< 0x7D: Shift right long (lógico).
void exec_iand(Thread *, Frame &);                 ///< 0x7E: AND bit a bit int.
void exec_land(Thread *, Frame &);                 ///< 0x7F: AND bit a bit long.
void exec_ior(Thread *, Frame &);                  ///< 0x80: OR bit a bit int.
void exec_lor(Thread *, Frame &);                  ///< 0x81: OR bit a bit long.
void exec_ixor(Thread *, Frame &);                 ///< 0x82: XOR bit a bit int.
void exec_lxor(Thread *, Frame &);                 ///< 0x83: XOR bit a bit long.
void exec_iinc(Thread *, Frame &);                 ///< 0x84: Incrementa variável local.

// Conversions
void exec_i2l(Thread *, Frame &);                  ///< 0x85: Converte int para long.
void exec_i2f(Thread *, Frame &);                  ///< 0x86: Converte int para float.
void exec_i2d(Thread *, Frame &);                  ///< 0x87: Converte int para double.
void exec_l2i(Thread *, Frame &);                  ///< 0x88: Converte long para int.
void exec_l2f(Thread *, Frame &);                  ///< 0x89: Converte long para float.
void exec_l2d(Thread *, Frame &);                  ///< 0x8A: Converte long para double.
void exec_f2i(Thread *, Frame &);                  ///< 0x8B: Converte float para int.
void exec_f2l(Thread *, Frame &);                  ///< 0x8C: Converte float para long.
void exec_f2d(Thread *, Frame &);                  ///< 0x8D: Converte float para double.
void exec_d2i(Thread *, Frame &);                  ///< 0x8E: Converte double para int.
void exec_d2l(Thread *, Frame &);                  ///< 0x8F: Converte double para long.
void exec_d2f(Thread *, Frame &);                  ///< 0x90: Converte double para float.
void exec_i2b(Thread *, Frame &);                  ///< 0x91: Converte int para byte.
void exec_i2c(Thread *, Frame &);                  ///< 0x92: Converte int para char.
void exec_i2s(Thread *, Frame &);                  ///< 0x93: Converte int para short.

// Comparisons
void exec_lcmp(Thread *, Frame &);                 ///< 0x94: Compara long.
void exec_fcmpl(Thread *, Frame &);                ///< 0x95: Compara float (NaN é menor).
void exec_fcmpg(Thread *, Frame &);                ///< 0x96: Compara float (NaN é maior).
void exec_dcmpl(Thread *, Frame &);                ///< 0x97: Compara double (NaN é menor).
void exec_dcmpg(Thread *, Frame &);                ///< 0x98: Compara double (NaN é maior).
void exec_ifeq(Thread *, Frame &);                 ///< 0x99: Branch se int == 0.
void exec_ifne(Thread *, Frame &);                 ///< 0x9A: Branch se int != 0.
void exec_iflt(Thread *, Frame &);                 ///< 0x9B: Branch se int < 0.
void exec_ifge(Thread *, Frame &);                 ///< 0x9C: Branch se int >= 0.
void exec_ifgt(Thread *, Frame &);                 ///< 0x9D: Branch se int > 0.
void exec_ifle(Thread *, Frame &);                 ///< 0x9E: Branch se int <= 0.
void exec_if_icmpeq(Thread *, Frame &);            ///< 0x9F: Branch se int1 == int2.
void exec_if_icmpne(Thread *, Frame &);            ///< 0xA0: Branch se int1 != int2.
void exec_if_icmplt(Thread *, Frame &);            ///< 0xA1: Branch se int1 < int2.
void exec_if_icmpge(Thread *, Frame &);            ///< 0xA2: Branch se int1 >= int2.
void exec_if_icmpgt(Thread *, Frame &);            ///< 0xA3: Branch se int1 > int2.
void exec_if_icmple(Thread *, Frame &);            ///< 0xA4: Branch se int1 <= int2.
void exec_if_acmpeq(Thread *, Frame &);            ///< 0xA5: Branch se ref1 == ref2.
void exec_if_acmpne(Thread *, Frame &);            ///< 0xA6: Branch se ref1 != ref2.

// Control Flow 
void exec_goto(Thread *, Frame &);                 ///< 0xA7: Salto incondicional.
void exec_jsr(Thread *, Frame &);                  ///< 0xA8: Jump subroutine.
void exec_ret(Thread *, Frame &);                  ///< 0xA9: Return from subroutine.
void exec_tableswitch(Thread *, Frame &);          ///< 0xAA: Switch case (tabela).
void exec_lookupswitch(Thread *, Frame &);         ///< 0xAB: Switch case (busca).
void exec_ireturn(Thread *, Frame &);              ///< 0xAC: Retorna int.
void exec_lreturn(Thread *, Frame &);              ///< 0xAD: Retorna long.
void exec_freturn(Thread *, Frame &);              ///< 0xAE: Retorna float.
void exec_dreturn(Thread *, Frame &);              ///< 0xAF: Retorna double.
void exec_areturn(Thread *, Frame &);              ///< 0xB0: Retorna referência.
void exec_return(Thread *, Frame &);               ///< 0xB1: Retorna void.

// References & Invocations
void exec_getstatic(Thread *, Frame &);            ///< 0xB2: Lê campo estático.
void exec_putstatic(Thread *, Frame &);            ///< 0xB3: Escreve campo estático.
void exec_getfield(Thread *, Frame &);             ///< 0xB4: Lê campo de instância.
void exec_putfield(Thread *, Frame &);             ///< 0xB5: Escreve campo de instância.
void exec_invokevirtual(Thread *, Frame &);        ///< 0xB6: Invoca método de instância.
void exec_invokespecial(Thread *, Frame &);        ///< 0xB7: Invoca construtor ou método privado.
void exec_invokestatic(Thread *, Frame &);         ///< 0xB8: Invoca método estático.
void exec_invokeinterface(Thread *, Frame &);      ///< 0xB9: Invoca método de interface.
void exec_invokedynamic(Thread *, Frame &);        ///< 0xBA: Invoca dinamicamente (Java 7+).
void exec_new(Thread *, Frame &);                  ///< 0xBB: Cria nova instância de objeto.
void exec_newarray(Thread *, Frame &);             ///< 0xBC: Cria array de tipo primitivo.
void exec_anewarray(Thread *, Frame &);            ///< 0xBD: Cria array de referências.
void exec_arraylength(Thread *, Frame &);          ///< 0xBE: Obtém tamanho do array.
void exec_athrow(Thread *, Frame &);               ///< 0xBF: Lança exceção.
void exec_checkcast(Thread *, Frame &);            ///< 0xC0: Verifica tipo do objeto.
void exec_instanceof(Thread *, Frame &);           ///< 0xC1: Verifica 'instanceof'.
void exec_monitorenter(Thread *, Frame &);         ///< 0xC2: Entra em monitor (sync).
void exec_monitorexit(Thread *, Frame &);          ///< 0xC3: Sai de monitor (sync).
void exec_wide(Thread *, Frame &);                 ///< 0xC4: Extende índice de variável local.
void exec_multianewarray(Thread *, Frame &);       ///< 0xC5: Cria array multidimensional.
void exec_ifnull(Thread *, Frame &);               ///< 0xC6: Branch se null.
void exec_ifnonnull(Thread *, Frame &);            ///< 0xC7: Branch se não null.
void exec_goto_w(Thread *, Frame &);               ///< 0xC8: Salto longo (wide).
void exec_jsr_w(Thread *, Frame &);                ///< 0xC9: Jump subroutine longo (wide).
