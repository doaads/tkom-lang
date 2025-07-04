#include <cmath>
#include "builtin.h"
#include "builtin_helpers.h"
#include "builtin_defines.h"

/**
 * @brief function printing a string into the standard output
 */
auto __stdout(ArgVector& args) -> ValType {
    ValType string = get_value(args[0]);
    std::cout << std::get<std::string>(string);
    return std::monostate();  // void
}

/**
 * @brief function incrementing a mutable integer
 */
auto __increment_r(ArgVector& args) -> ValType {
    ValType arg = get_value(args[0]);
    modify_value(args[0], std::get<int>(arg) + 1);
    return std::monostate();  // void
}

auto __increment_v(ArgVector& args) -> ValType {
    ValType arg = get_value(args[0]);
    return std::get<int>(arg) + 1;
}

/**
 * @brief function decrementing a mutable integer
 */
auto __decrement(ArgVector& args) -> ValType {
    ValType arg = get_value(args[0]);
    modify_value(args[0], std::get<int>(arg) - 1);
    return std::monostate();  // void
}

auto __stdin(ArgVector& args) -> ValType {
    std::string val;
    ValType string = get_value(args[0]);
    std::cout << std::get<std::string>(string);
    std::cin >> val;
    return val;
}

auto __sqrt_r(ArgVector& args) -> ValType {
    ValType arg = get_value(args[0]);
    modify_value(args[0], sqrt(std::get<double>(arg)));
    return std::monostate();
}

auto __sqrt_v(ArgVector& args) -> ValType {
    ValType arg = get_value(args[0]);
    return sqrt(std::get<double>(arg));
}

/**
 * @brief vector containing an implementation of builtin functions
 */
const std::vector<std::shared_ptr<Callable>> builtins = {
    std::make_shared<BuiltIn>(
        __stdout, "stdout",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::STRING, false)})),
    std::make_shared<BuiltIn>(
        __increment_r, "increment",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::INT, true)})),
    std::make_shared<BuiltIn>(
        __increment_v, "increment_v",
        build_type({VarType(BaseType::INT, false), VarType(BaseType::INT, false)})),
    std::make_shared<BuiltIn>(
        __sqrt_r, "sqrt",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::FLT, true)})),
    std::make_shared<BuiltIn>(
        __stdin, "stdin",
        build_type({VarType(BaseType::STRING, false), VarType(BaseType::STRING, false)})),
    std::make_shared<BuiltIn>(
        __sqrt_v, "sqrt_v",
        build_type({VarType(BaseType::FLT, false), VarType(BaseType::FLT, false)})),
};
