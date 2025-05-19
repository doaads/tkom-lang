#pragma once

#include "visitor.h"

using ValueType = std::variant<std::monostate, std::string, int, double, bool>;
class Callable;

class InterpreterVisitor : public Visitor {
    private:
        ValueType current_value;
        ValueType decorate(ValueType decorator, ValueType decoratee);
    public:
        void visit(const Program &program);

        void visit(const LiteralExpr &expr);
        void visit(const IdentifierExpr &expr);
        void visit(const UnaryExpr &expr);
        void visit(const BinaryExpr &expr);
        void visit(const CallExpr &expr);
        void visit(const BindFrtExpr &expr);

        void visit(const ForLoopStatement &stmt);
        void visit(const WhileLoopStatement &stmt);
        void visit(const ConditionalStatement &stmt);
        void visit(const ElseStatement &stmt);
        void visit(const RetStatement &stmt);
        void visit(const CallStatement &stmt);
        void visit(const AssignStatement &stmt);

        void visit(const Block &block);
        void visit(const VarType &var);
        void visit(const FuncType &func);

        void visit(const FuncParam &var);
        void visit(const FuncSignature &sign);
        void visit(const Function &func);
};


void InterpreterVisitor::visit(const LiteralExpr &expr) {
    current_value = expr.get_value<ValueType>();
}

void InterpreterVisitor::visit(const BinaryExpr &expr) {
    expr.get_right()->accept(*this);
    ValueType right = current_value;
    
    expr.get_left()->accept(*this);
    ValueType left = current_value;

    switch (expr.get_operator()) {
        case BinaryOp::DECORATE:
            current_value = decorate(left, right);  // TODO: add to function vector in current context
            break;
        default:
            throw std::runtime_error("Unimplemented operator");
    }
}
