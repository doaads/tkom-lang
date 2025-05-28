#pragma once

#include "interpreter.h"
#include <deque>

void modify_value(Arg& arg, ValType value);

ValType get_value(Arg& arg);

std::unique_ptr<Type> build_type(std::deque<VarType> args);

