#include "interpreter.h"
#include "type_cast.h"

void InterpreterVisitor::register_function(const Function* func) {
    auto func_ptr = std::make_shared<GlobalFunction>(func);
    functions.push_back(std::move(func_ptr));
}

ValType InterpreterVisitor::get_value() const { return current_value; }

void InterpreterVisitor::push_call_stack(CallStackFrame frame) { call_stack.push_back(frame); }

void InterpreterVisitor::pop_call_stack() { return call_stack.pop_back(); }

std::weak_ptr<Variable> InterpreterVisitor::find_var_in_frame(const std::string& name) {
    CallStackFrame& frame = call_stack.back();

    // find in local vars
    for (auto it = frame.var_scope.rbegin(); it != frame.var_scope.rend(); ++it) {
        auto& block = *it;
        auto var_it = std::find_if(block.vars.begin(), block.vars.end(),
                                   [&name](const std::shared_ptr<Variable>& var) {
                                       return var->signature.get_name() == name;
                                   });
        if (var_it != block.vars.end()) {
            return *var_it;
        }
    }

    // find in function args (references)
    auto ref_var = std::find_if(frame.args.begin(), frame.args.end(),
                                [&name](const auto& arg) { return arg->curr_name == name; });

    if (ref_var < frame.args.end()) return ref_var->get()->ref;

    return {};
}

std::shared_ptr<Callable> InterpreterVisitor::find_func(const std::string& name) {
    auto func = std::find_if(functions.begin(), functions.end(), [&name](const auto& func) {
        return func->get_name() == name;
    });

    if (func < functions.end()) return *func;
    return nullptr;
}

void InterpreterVisitor::modify_var(const std::string& identifier) {
    ValType value = current_value;
    std::shared_ptr<Variable> var = find_var_in_frame(identifier).lock();

    if (!var) throw std::runtime_error("Variable not in scope");

    const Type* type = var->signature.get_type();
    if (!type->get_mut()) throw std::runtime_error("Immutable variables cannot be reassigned");

    var->value = std::visit(TypeCast(), value, var->value);
}

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;
ValType InterpreterVisitor::init_var(const Type& type) {
    type.accept(*this);
    return std::visit(Overload{[](const Type*) -> ValType { return ValType{nullptr}; },
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
                                           throw std::runtime_error("Expected non-void type");
                                   }
                                   throw std::runtime_error("Unable to initalize variable");
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

bool InterpreterVisitor::eval_condition(const Expression& expr) {
    expr.accept(*this);
    return std::get<bool>(std::visit(TypeCast(), current_value, ValType{true}));
}

std::weak_ptr<Variable> InterpreterVisitor::get_for_iterator(const ForLoopArgs& args) {
    return std::visit(Overload{[this](const std::unique_ptr<Statement>& iterator) {
                                iterator->accept(*this);
                                return this->call_stack.back().var_scope.back().vars.back();
                            },
                            [this](const std::string& iterator) {
                                auto var = find_var_in_frame(iterator);
                                if (!var.lock()) throw std::runtime_error("Invalid iterator");
                                return var.lock();
                            }},
                   args.iterator);
}

