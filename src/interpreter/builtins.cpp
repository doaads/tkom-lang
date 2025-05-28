#include "builtin.h"
#include "builtin_helpers.h"
#include "builtin_defines.h"

/*
 * @brief: function printing a string into the standard output
 */
void __stdout(ArgVector& args) {
    shall(args.size() == 1, "Expected 1 argument");
    ValType string = get_value(args[0]);
    shall(std::holds_alternative<std::string>(string), "Expected string");
    std::cout << std::get<std::string>(string);
}

/*
 * @brief: function incrementing a mutable integer
 */
void __increment(ArgVector& args) {
    shall(args.size() == 1, "Expected 1 argument");
    ValType arg = get_value(args[0]);
    shall(std::holds_alternative<int>(arg), "Expected int");
    modify_value(args[0], std::get<int>(arg) + 1);
}

/*
 * @brief: vector containing an implementation of builtin functions
 */
const std::vector<std::shared_ptr<Callable>> builtins = {
    std::make_shared<BuiltIn>(
        __stdout, "stdout",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::STRING, false)})),
    std::make_shared<BuiltIn>(
        __increment, "increment",
        build_type({VarType(BaseType::VOID, false), VarType(BaseType::INT, true)})),
};
