#include "builtin.h"
#include "builtin_helpers.h"
#include "builtin_defines.h"

/**
 * @brief function printing a string into the standard output
 */
ValType __stdout(ArgVector& args) {
    ValType string = get_value(args[0]);
    std::cout << std::get<std::string>(string);
    return std::monostate();  // void
}

/**
 * @brief function incrementing a mutable integer
 */
ValType __increment(ArgVector& args) {
    ValType arg = get_value(args[0]);
    modify_value(args[0], std::get<int>(arg) + 1);
    return std::monostate();  // void
}

/**
 * @brief function decrementing a mutable integer
 */
ValType __decrement(ArgVector& args) {
    ValType arg = get_value(args[0]);
    modify_value(args[0], std::get<int>(arg) - 1);
    return std::monostate();  // void
}

ValType __stdin(ArgVector& args) {
    std::string val;
    ValType string = get_value(args[0]);
    std::cout << std::get<std::string>(string);
    std::cin >> val;
    return val;
}

/**
 * @brief vector containing an implementation of builtin functions
 */
const std::vector<std::shared_ptr<Callable>> builtins = {
    std::make_shared<BuiltIn>(
        __stdout, "stdout",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::STRING, false)})),
    std::make_shared<BuiltIn>(
        __increment, "increment",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::INT, true)})),
    std::make_shared<BuiltIn>(
        __stdin, "stdin",
        build_type({VarType(BaseType::STRING, false), VarType(BaseType::STRING, false)})),
};
