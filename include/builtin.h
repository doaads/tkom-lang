#pragma once
#include <functional>
#include <utility>

#include "interpreter.h"
#include "interpreter_shall.h"
#include "local_function.h"
#include "type.h"
#include "type_cast.h"

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

/**
 * @brief the function to be called
 *
 * The function must accept an `ArgVector` reference and return `ValType`
 */
using builtin = std::function<ValType(ArgVector&)>;

/**
 * @brief class representing a builtin-function wrapper allowing it to be stored in the interpreter
 */
class BuiltIn : public Callable {
   private:
    builtin func;  //< the function to be called
    std::string name;  //< the name of the function
    std::unique_ptr<Type> type;  //< the type signature of the function

   public:
    BuiltIn(builtin f, std::string name, std::unique_ptr<Type> t)
        : func(std::move(f)), name(std::move(name)), type(std::move(t)) {}

    /**
     * @brief call the builtin function
     * @param interpreter the interpreter to be used for type-checking
     * @param params the function's parameters as an `ArgVector`
     */
    void call(InterpreterVisitor& interpreter, ArgVector params) override {
        check_types(interpreter, params);
        auto retval = func(params);
        interpreter.override_value(retval);
    }

    /**
     * @brief retrieving the pointer to the function is impossible, as builtin functions are not user-defined
     */
    [[nodiscard]] auto get_func() const -> const Function* override {
        throw std::runtime_error("Cannot retrieve builtin func");
    }
    /**
     * @brief get the function's type
     */
    [[nodiscard]] auto get_type() const -> const Type* override { return type.get(); }
    /**
     * @brief get the function's name as a `std::string`
     */
    [[nodiscard]] auto get_name() const -> const std::string override { return name; }

    /**
     * @brief check the function's argument types
     *
     * This method checks for type equality when dealing with references, and creates
     * temporary variables for values, attempting to type cast them if needed
     *
     * @param interpreter the interpreter to be used for variable initializing
     * @param args function's argument vector
     */
    auto check_types(InterpreterVisitor& interpreter, ArgVector& args) const
        -> std::vector<std::shared_ptr<VarRef>> {
        const auto expected = type->get_params();
        shall(expected.size() == args.size(), "Incorrect number of arguments");
        std::vector<std::shared_ptr<VarRef>> var_refs;
        for (size_t i = 0; i < expected.size(); ++i) {
            auto expected_arg = expected[i];
            std::visit(
                Overload{[&](std::shared_ptr<Variable> var) {
                             shall(expected_arg->is_equal_to(var->get_type()),
                                   "Type-mismatched reference. Did you want to pass by value?");
                         },
                         [&](ValType& value) {
                             auto type = expected[i];
                             value = std::visit(TypeCast(), value, interpreter.init_var(*type));
                         }},
                args[i]);
        }
        return var_refs;
    }
};
