#include <memory>

#include "interpreter.h"

void InterpreterVisitor::visit(const Program& program) {
    for (const auto& fn : program.get_functions()) {
        register_function(fn);
    }
    auto main = find_func("main");
    main->call(*this, {});
}

void InterpreterVisitor::visit(const Function& func) {
    func.get_body()->accept(*this);
    if (returning) returning = false;
}

void InterpreterVisitor::visit(const Block& block) {
    call_stack.back().var_scope.push_back(BlockScope());
    for (auto& stmt : block.get_statements()) {
        stmt->accept(*this);
        if (returning) break;
    }
    call_stack.back().var_scope.pop_back();
}

void InterpreterVisitor::visit(const CallExpr& expr) {
    ArgVector args;
    expr.get_func_name()->accept(*this);
    auto func = current_value;

    for (auto& arg : expr.get_args()) {
        arg->accept(*this);
        // we got a identifier expression, pass by ref
        if (receiver == ReceivedBy::VAR) {
            auto var_ptr = var;
            args.push_back(var_ptr.lock());
        // we got a regular expression, we will pass by value
        } else {
            args.push_back(current_value);
        }
    }
    std::get<std::shared_ptr<Callable>>(func)->call(*this, args);
}

void InterpreterVisitor::visit(const IdentifierExpr& expr) {
    std::weak_ptr<Variable> found = find_var_in_frame(expr.get_identifier());

    if (auto var_val = found.lock()) {
        current_value = var_val->value;
        var = found;
    } else {
        auto func = find_func(expr.get_identifier());
        if (!func) throw std::runtime_error("Unknown identifier");
        current_value = func;
    }
    receiver = ReceivedBy::VAR;
}

void InterpreterVisitor::visit(const AssignStatement& stmt) {
    stmt.get_value()->accept(*this);
    ValType value = current_value;

    auto type = stmt.get_type();
    const std::string identifier = stmt.get_identifier();

    if (!type) {
        modify_var(identifier);
    } else {
        register_var(*stmt.get_signature());
    }
}

void InterpreterVisitor::visit(const VarType& type) { current_type = type.get_type(); }

void InterpreterVisitor::visit(const WhileLoopStatement& stmt) {
    auto condition = stmt.get_condition();
    while (eval_condition(*condition)) {
        stmt.get_body()->accept(*this);
    }
}

void InterpreterVisitor::visit(const ConditionalStatement& stmt) {
    auto condition = stmt.get_condition();
    if (eval_condition(*condition)) {
        stmt.get_body()->accept(*this);
    } else if (auto else_st = stmt.get_else_st()) {
        else_st->accept(*this);
    }
}

void InterpreterVisitor::visit(const ElseStatement& stmt) { stmt.get_body()->accept(*this); }

void InterpreterVisitor::visit(const ForLoopStatement& stmt) {
    auto& args = *stmt.get_args();
    auto condition = args.condition.get();
    auto iterator = get_for_iterator(args);

    stmt.get_on_iter()->accept(*this);
    ValType on_iter = current_value;


    if (std::holds_alternative<std::shared_ptr<Callable>>(on_iter))
        throw std::runtime_error("on iter call identifier is not a valid function");

    auto on_iter_val = std::get<std::shared_ptr<Callable>>(on_iter);
    ArgVector on_iter_arg = {iterator.lock()};
    LocalFunction on_iter_func = LocalFunction(on_iter_val, {});

    for (;eval_condition(*condition); on_iter_func.call(*this, on_iter_arg)){
        stmt.get_body()->accept(*this);
    }
}

void InterpreterVisitor::visit(const RetStatement& stmt) { 
    stmt.get_retval()->accept(*this);
    returning = true;
}

void InterpreterVisitor::visit(const CallStatement& stmt) {
    stmt.get_call()->accept(*this);
}
