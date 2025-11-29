#pragma once

#include <unordered_map>
#include <string>
#include "runtime_class_types.h"
#include "array.h"

using NativeMethod = void (*)(Frame &frame);

void load_map(std::unordered_map<std::string, NativeMethod> *native_methods);

void print_ln(Frame &frame);
void print_str_ln(Frame &frame);
void print_int_ln(Frame &frame);
void print_lg_ln(Frame &frame);
void print_bl_ln(Frame &frame);
void print_fl_ln(Frame &frame);
void print_db_ln(Frame &frame);

void str_len(Frame &frame);
void str_char_at(Frame &frame);
void str_eq(Frame &frame);
void str_get_bytes(Frame &frame);
