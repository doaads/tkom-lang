#include <algorithm>
#include <ranges>

#include "interpreter.h"
#include "interpreter_shall.h"
#include "type_cast.h"

void InterpreterVisitor::register_function(const Function* func) {
    auto func_ptr = std::make_shared<GlobalFunction>(func);
    functions.push_back(std::move(func_ptr));
}

auto InterpreterVisitor::get_value() const -> ValType { return current_value; }

void InterpreterVisitor::override_value(ValType val) { current_value = val; }

void InterpreterVisitor::push_call_stack(CallStackFrame frame) { call_stack.push_back(frame); }

void InterpreterVisitor::pop_call_stack() { return call_stack.pop_back(); }

auto InterpreterVisitor::find_var_in_frame(const std::string& name) -> std::weak_ptr<Variable> {
    CallStackFrame& frame = call_stack.back();

    // find in local vars
    for (auto& block : std::ranges::reverse_view(frame.var_scope)) {
        auto var_it =
            std::ranges::find_if(block.vars, [&name](const std::shared_ptr<Variable>& var) {
                return var->signature.get_name() == name;
            });
        if (var_it != block.vars.end()) {
            return *var_it;
        }
    }

    // find in function args (references)
    auto ref_var = std::ranges::find_if(
        frame.args, [&name](const auto& arg) { return arg->curr_name == name; });

    if (ref_var < frame.args.end()) return ref_var->get()->ref;

    return {};
}

auto InterpreterVisitor::find_func(const std::string& name) -> std::shared_ptr<Callable> {
    auto func = std::ranges::find_if(
        functions, [&name](const auto& func) { return func->get_name() == name; });

    if (func < functions.end()) return *func;
    return nullptr;
}

void InterpreterVisitor::modify_var(const std::string& identifier) {
    ValType value = current_value;
    std::shared_ptr<Variable> var = find_var_in_frame(identifier).lock();

    shall(var != nullptr, "Variable not in scope: " + identifier);

    const Type* type = var->signature.get_type();
    shall(type->get_mut(), "Immutable variables cannot be reassigned");

    var->value = std::visit(TypeCast(), value, var->value);
}

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;
auto InterpreterVisitor::init_var(const Type& type) -> ValType {
    type.accept(*this);
    return std::visit(Overload{[](const Type* type) -> ValType {
                                   return std::make_shared<LocalFunction>(type->clone());
                               },
                               [](BaseType type) -> ValType {
                                   switch (type) {
                                       case BaseType::INT:
                                           return 0;
                                       case BaseType::FLT:
                                           return 0.0;
                                       case BaseType::BOOL:
                                           return true;
                                       case BaseType::STRING:
                                           return "";
                                       case BaseType::VOID:
                                           return std::monostate();
                                   }
                                   throw InterpreterError("Unable to initialize variable");
                               }},
                      current_type);
}

void InterpreterVisitor::register_var(const VariableSignature& signature) {
    ValType value = current_value;
    ValType var_val = init_var(*signature.get_type());
    var_val = std::visit(TypeCast(), value, var_val);
    call_stack.back().var_scope.back().vars.push_back(
        std::make_shared<Variable>(signature, var_val));
}

auto InterpreterVisitor::eval_condition(const Expression& expr) -> bool {
    expr.accept(*this);
    return std::get<bool>(std::visit(TypeCast(), current_value, ValType{true}));
}

auto InterpreterVisitor::get_for_iterator(const ForLoopArgs& args) -> std::weak_ptr<Variable> {
    return std::visit(Overload{[this](const std::unique_ptr<Statement>& iterator) {
                                   iterator->accept(*this);
                                   return this->call_stack.back().var_scope.back().vars.back();
                               },
                               [this](const std::string& iterator) {
                                   auto var = find_var_in_frame(iterator);
                                   shall(var.lock(), "Invalid iterator");
                                   return var.lock();
                               }},
                      args.iterator);
}
