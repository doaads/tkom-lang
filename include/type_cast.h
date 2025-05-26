#pragma once

#include "interpreter.h"

struct TypeCast {
    template <typename T>
    ValType operator()(T a, T) {
        return a;
    }

    ValType operator()(int a, double) { return static_cast<double>(a); }
    ValType operator()(double a, int) { return static_cast<int>(a); }

    ValType operator()(std::string, int) {
        throw std::runtime_error("int type cannot hold a string");
    }
    ValType operator()(int a, std::string) { return std::to_string(a); }

    ValType operator()(std::string, double) {
        throw std::runtime_error("flt type cannot hold a string");
    }
    ValType operator()(double a, std::string) { return std::to_string(a); }

    ValType operator()(bool a, int) { return a ? 1 : 0; }
    ValType operator()(int a, bool) { return static_cast<bool>(a); }

    ValType operator()(bool a, double) { return a ? 1.0 : 0.0; }
    ValType operator()(double a, bool) { return static_cast<bool>(a); }
    ValType operator()(std::string a, bool) { return a.empty(); }

    template <typename A, typename B>  // we throw if we get 2 type combinations not mentioned here
    ValType operator()(A, B) { throw std::runtime_error("Cannot cast type"); }
};

