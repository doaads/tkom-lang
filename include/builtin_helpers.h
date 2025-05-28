#pragma once

#include "interpreter.h"
#include <deque>

void modify_value(Arg& arg, ValType value);

ValType get_value(Arg& arg);

template <typename T>
T shall(T arg, std::string error);

std::unique_ptr<Type> build_type(std::deque<VarType> args);

