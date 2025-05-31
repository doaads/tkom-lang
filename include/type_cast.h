#pragma once

#include "exceptions.h"
#include "local_function.h"

/**
 * @brief structure of call operators for type casting
 *
 * the structure consists of call operators, accepting two arguments which
 * are the current type, and the desired type
 */
struct TypeCast {
    /**
     * @brief when type casting functions, pass them on if their type is the same
     */
    auto operator()(std::shared_ptr<Callable> a, std::shared_ptr<Callable> b) -> ValType {
        if (a->get_type()->is_equal_to(b->get_type())) {
            return a;
        }
        throw InterpreterError("Functions have a different number of parameters");
    }

    /**
     * @brief if both types are the same, we don't need to type cast
     */
    template <typename T>
    auto operator()(T a, T) -> ValType {
        return a;
    }

    auto operator()(int a, double) -> ValType { return static_cast<double>(a); }
    auto operator()(double a, int) -> ValType { return static_cast<int>(a); }

    auto operator()(std::string a, int) -> ValType {
        try {
            int result = std::stoi(a);
            return result;
        } catch (std::exception& e) {
            throw InterpreterError("Cannot cast " + a + " to int");
        }
    }
    auto operator()(int a, std::string) -> ValType { return std::to_string(a); }

    auto operator()(std::string a, double) -> ValType {
        try {
            double result = std::stod(a);
            return result;
        } catch (std::exception& e) {
            throw InterpreterError("Cannot cast " + a + " to double");
        }
    }
    auto operator()(double a, std::string) -> ValType { return std::to_string(a); }

    auto operator()(bool a, int) -> ValType { return a ? 1 : 0; }
    auto operator()(int a, bool) -> ValType { return static_cast<bool>(a); }

    auto operator()(bool a, double) -> ValType { return a ? 1.0 : 0.0; }
    auto operator()(double a, bool) -> ValType { return static_cast<bool>(a); }
    auto operator()(std::string a, bool) -> ValType { return a.empty(); }

    /**
     * @brief throw an exception if we get two differing types not mentioned above
     */
    template <typename A, typename B>
    auto operator()(A, B) -> ValType {
        throw InterpreterError("Cannot cast type");
    }
};

