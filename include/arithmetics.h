#pragma once
#include "exceptions.h"
#include "interpreter_helpers.h"

/**
 * @brief function for promoting types to string for the purpose of weak-type arithmetics
 */
template <typename T>
std::string to_string(const T& v) {
    if constexpr (std::is_same_v<T, bool>) {
        return v ? "true" : "false";
    } else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(v);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return v;
    } else {
        throw InterpreterError("Unsupported type");
    }
}

/**
 * @brief function for checking if types are of arithmetic rank for 2-argument functions
 */
template <typename L, typename R>
constexpr bool are_of_arithmetic_rank() {
    return std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;
}

/**
 * @brief function returning the value of an arithmetic function provided as the third argument
 */
template <typename OpFunc>
ValType get_val_for_arithmetic(const ValType& l, const ValType& r, OpFunc func) {
    return std::visit(
        [func](auto l, auto r) -> ValType {
            if constexpr (are_of_arithmetic_rank<decltype(l), decltype(r)>()) return func(l, r);
            throw InterpreterError("Unsupported operator for type");
        },
        l, r);
}

/**
 * @brief overload template for expression parsing
 *
 * Inspired by: https://www.modernescpp.com/index.php/smart-tricks-with-fold-expressions/
 */
template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

/**
 * @brief struct for handling the '+' operator
 */
struct Add {
    ValType operator()(const std::string& l, const std::string& r) { return l + r; }
    ValType operator()(const std::string& l, auto r) { return l + to_string(r); }
    ValType operator()(auto l, const std::string& r) { return to_string(l) + r; }
    ValType operator()(const auto& l, const auto& r) {
        return get_val_for_arithmetic(
            l, r, [](const auto& a, const auto& b) -> ValType { return a + b; });
    }
};

/*
 * @brief struct for handling the '-' operator
 *
 * the function returns a value only for arithmetic types
 */

struct Sub {
    ValType operator()(auto l, auto r) {
        return get_val_for_arithmetic(l, r, [](auto a, auto b) -> ValType { return a - b; });
    }
};

/**
 * @brief struct for handling the '*' operator
 *
 * arithmetic types are multiplied normally, (string, int) combination returns a n-times multiplied
 * string
 */
struct Mul {
    ValType mult_string(const std::string& str, int count) {
        if (count < 0) throw InterpreterError("Cannot multiply a string by a negative integer");
        std::string result;
        for (int i = 0; i < count; ++i) result += str;
        return result;
    }
    ValType operator()(const std::string& s, int count) { return mult_string(s, count); }
    ValType operator()(int count, const std::string& s) { return mult_string(s, count); }
    ValType operator()(const auto& l, const auto& r) {
        return get_val_for_arithmetic(
            l, r, [](const auto& a, const auto& b) -> ValType { return a * b; });
    }
};

/**
 * @brief struct for handling the '/' operator
 *
 * works only for arithmetic types
 */
struct Div {
    ValType operator()(const auto& l, const auto& r) {
        return get_val_for_arithmetic(
            l, r, [](const auto& a, const auto& b) -> ValType { return a / b; });
    }
};

/**
 * @brief function for handling comparison operators (<, >, <=, >=, ==, !=)
 *
 * string comparison only compares string length for both arguments
 */
template <typename CompareFunc>
struct Compare {
    CompareFunc func;

    ValType operator()(const std::string& l, const std::string& r) { return func(l, r); }
    ValType operator()(const auto& l, const auto& r) { return get_val_for_arithmetic(l, r, func); }
};

/**
 * @brief function for handling unary operators (!, -)
 */
template <typename UnaryFunc>
struct Unary {
    UnaryFunc func;
    ValType operator()(auto l) {
        if constexpr (std::is_arithmetic_v<decltype(l)>) return func(l);
        throw InterpreterError("Unsupported operator for type");
    }
};

/**
 * @brief struct for handling logical operators (&&, ||)
 */
template <typename LogicalFunc>
struct Logical {
    LogicalFunc func;

    ValType operator()(const std::string& l, const std::string& r) {
        return func(!l.empty(), !r.empty());
    }
    ValType operator()(const auto& l, const auto& r) { return get_val_for_arithmetic(l, r, func); }
};

struct Decorate {
    ValType operator()(const std::shared_ptr<Callable>& l, const std::shared_ptr<Callable>& r) {
        ArgVector args;
        args.push_back(ValType{l});
        return std::make_shared<LocalFunction>(r, args);
    }
    ValType operator()(auto, auto) { throw InterpreterError("Unsupported operator: '@'"); }
};
