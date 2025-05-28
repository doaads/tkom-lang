#pragma once
#include "exceptions.h"

template <typename T>
T shall(T arg, std::string error) {
    if (!arg) throw InterpreterError(error);
    return std::move(arg);
}

