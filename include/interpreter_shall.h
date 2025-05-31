#pragma once
#include "exceptions.h"

template <typename T>
auto shall(T arg, std::string error) -> T {
    if (!arg) throw InterpreterError(error);
    return std::move(arg);
}
