#pragma once

#include "interpreter.h"

struct TypeCast {
    ValType operator()(std::shared_ptr<Callable> a, std::shared_ptr<Callable> b) {
        if (a->get_type()->is_equal_to(b->get_type())) {
            return a;
        }
        throw InterpreterError("Functions have a different number of parameters");
    }

    template <typename T>
    ValType operator()(T a, T) {
        return a;
    }

    ValType operator()(int a, double) { return static_cast<double>(a); }
    ValType operator()(double a, int) { return static_cast<int>(a); }

    ValType operator()(std::string a, int) {
        try {
            int result = std::stoi(a);
            return result;
        } catch (std::exception& e) {
            throw InterpreterError("Cannot cast " + a + " to int");
        }
    }
    ValType operator()(int a, std::string) { return std::to_string(a); }

    ValType operator()(std::string a, double) {
        try {
            int result = std::stod(a);
            return result;
        } catch (std::exception& e) {
            throw InterpreterError("Cannot cast " + a + " to int");
        }
        throw InterpreterError("flt type cannot hold a string");
    }
    ValType operator()(double a, std::string) { return std::to_string(a); }

    ValType operator()(bool a, int) { return a ? 1 : 0; }
    ValType operator()(int a, bool) { return static_cast<bool>(a); }

    ValType operator()(bool a, double) { return a ? 1.0 : 0.0; }
    ValType operator()(double a, bool) { return static_cast<bool>(a); }
    ValType operator()(std::string a, bool) { return a.empty(); }

    template <typename A, typename B>  // we throw if we get 2 type combinations not mentioned here
    ValType operator()(A, B) { throw InterpreterError("Cannot cast type"); }
};

