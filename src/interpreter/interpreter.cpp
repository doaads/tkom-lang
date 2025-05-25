#include "interpreter.h"

void InterpreterVisitor::register_function(const Function* func) {
    auto func_ptr = std::make_shared<GlobalFunction>(func);
    functions.push_back(std::move(func_ptr));
}

void InterpreterVisitor::visit(const Program& program) {
    for (const auto& fn : program.get_functions()) {
        register_function(fn);

        auto signature = fn->get_signature();
        if (signature->get_name() == "main") {
            fn->accept(*this);  // call main
        }
    }
}

void InterpreterVisitor::visit(const Function& func) { func.get_body()->accept(*this); }

ValType InterpreterVisitor::get_value() const { return current_value; }

void InterpreterVisitor::push_call_stack(CallStackFrame frame) { call_stack.push_back(frame); }

void InterpreterVisitor::pop_call_stack() { return call_stack.pop_back(); }

void InterpreterVisitor::visit(const Block& block) {
    for (auto& stmt : block.get_statements()) {
        stmt->accept(*this);
    }
}

// TODO: figure out reference passing (only if arg is identifier expr? otherwise just eval and pass)
void InterpreterVisitor::visit(const CallExpr& expr) { (void)expr; }

std::weak_ptr<Variable> InterpreterVisitor::find_var_in_frame(const std::string& name) {
    CallStackFrame frame = call_stack.back();

    // find in local vars
    auto local_var = std::find_if(frame.vars.begin(), frame.vars.end(),
            [name](const std::shared_ptr<Variable>& var) {
                return var->signature->get_name() == name;});

    if (local_var < frame.vars.end()) return *local_var;

    // find in function args (references)
    auto ref_var = std::find_if(frame.args.begin(), frame.args.end(),
            [&name](const auto& arg) {
                return arg->curr_name == name;});

    if (ref_var < frame.args.end()) return ref_var->get()->ref;

    return {};
}

std::shared_ptr<Callable> InterpreterVisitor::find_func(const std::string& name) {
    auto func = std::find_if(functions.begin(), functions.end(),
            [&name](const auto& func) {
                return func->get_func()->get_signature()->get_name() == name;
            });

    if (func < functions.end()) return *func;
    return nullptr;
}

void InterpreterVisitor::visit(const IdentifierExpr &expr) {
    std::weak_ptr<Variable> var = find_var_in_frame(expr.get_identifier());

    if (auto var_val = var.lock()) {
        current_value = var_val->value;
    } else {
        throw std::runtime_error("Unknown identifier");
    }
}

struct TypeCast {
    template <typename T>
    ValType operator()(T a, T) { return a;}

    ValType operator()(int a, double) {return static_cast<double>(a);}
    ValType operator()(double a, int) {return static_cast<int>(a);}

    ValType operator()(std::string, int) {throw std::runtime_error("int type cannot hold a string");}
    ValType operator()(int a, std::string) {return std::to_string(a);}

    ValType operator()(std::string, double) {throw std::runtime_error("flt type cannot hold a string");}
    ValType operator()(double a, std::string) {return std::to_string(a);}

    ValType operator()(bool a, int) {return a ? 1 : 0;}
    ValType operator()(int a, bool) {return static_cast<bool>(a);}

    ValType operator()(bool a, double) {return a ? 1.0 : 0.0;}
    ValType operator()(double a, bool) {return static_cast<bool>(a);}

    ValType operator()(auto, auto) {throw std::runtime_error("Cannot cast type");}
};

void InterpreterVisitor::visit(const AssignStatement &stmt) {
    stmt.get_value()->accept(*this);
    ValType value = current_value;

    auto type = stmt.get_type();
    const std::string identifier = stmt.get_identifier();

    if (type == nullptr) {  // we have a variable mutation
        auto var = find_var_in_frame(identifier).lock();
        if (!var) throw std::runtime_error("Variable not in scope");
        var->value = std::visit(TypeCast(), value, var->value);
    }
}
