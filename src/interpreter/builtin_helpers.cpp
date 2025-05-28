#include "builtin_helpers.h"

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

/*
 * @brief: modify the value of a variable depending on if it is a reference or not
 */
void modify_value(Arg& arg, ValType value) {
    std::visit(Overload{[value](std::shared_ptr<Variable> var) { var->value = value; },
                        [value](ValType var) { var = value; }},
               arg);
}

/*
 * @brief: get the value of a variable depending on if it is a reference or not
 */
ValType get_value(Arg& arg) {
    return std::visit(Overload{[](std::shared_ptr<Variable> var) { return var->value; },
                               [](ValType var) { return var; }},
                      arg);
}

template <typename T>
T shall(T arg, std::string error) {
    if (!arg) throw std::runtime_error(error);
    return std::move(arg);
}

/*
 * @brief: construct a function type based on simple type definitions
 */
std::unique_ptr<Type> build_type(std::deque<VarType> args) {
    shall(!args.empty(), "BUILTINS: Expected return type");
    std::unique_ptr<Type> ret_type = std::make_unique<VarType>(args.front());
    args.pop_front();
    std::vector<std::unique_ptr<Type>> func_params;
    for (auto& arg : args) {
        std::unique_ptr<VarType> arg_ptr = std::make_unique<VarType>(arg);
        func_params.push_back(std::move(arg_ptr));
    }
    return std::make_unique<FuncType>(std::move(ret_type), std::move(func_params));
}


