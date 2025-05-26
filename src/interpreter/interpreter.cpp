#include <boost/smart_ptr/shared_ptr.hpp>
#include <memory>

#include "interpreter.h"

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

void InterpreterVisitor::visit(const Block& block) {
    call_stack.back().var_scope.push_back(BlockScope());
    for (auto& stmt : block.get_statements()) {
        stmt->accept(*this);
    }
    call_stack.back().var_scope.pop_back();
}

void InterpreterVisitor::visit(const CallExpr& expr) { (void)expr; }

void InterpreterVisitor::visit(const IdentifierExpr& expr) {
    std::weak_ptr<Variable> var = find_var_in_frame(expr.get_identifier());

    if (auto var_val = var.lock()) {
        current_value = var_val->value;
    } else {
        throw std::runtime_error("Unknown identifier");
    }
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
