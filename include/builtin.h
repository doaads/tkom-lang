#pragma once
#include <functional>

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

using builtin = std::function<ValType(ArgVector&)>;

class BuiltIn : public Callable {
   private:
    builtin func;
    std::string name;
    std::unique_ptr<Type> type;

   public:
    BuiltIn(builtin f, const std::string& name, std::unique_ptr<Type> t)
        : func(std::move(f)), name(name), type(std::move(t)) {}

    void call(InterpreterVisitor& interpreter, ArgVector params) override {
        check_types(interpreter, params);
        auto retval = func(params);
        interpreter.override_value(retval);
    }

    const Function* get_func() const override {
        throw std::runtime_error("Cannot retrieve builtin func");
    }
    const Type* get_type() const override { return type.get(); }
    const std::string get_name() const override { return name; }
    std::vector<std::shared_ptr<VarRef>> check_types(InterpreterVisitor& interpreter,
                                                     ArgVector& args) const {
        const auto expected = type->get_params();
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
