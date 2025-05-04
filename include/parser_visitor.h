#pragma once

#include <ostream>
#include "statement.h"
#include "expression.h"
#include "block.h"

class Expression;
class LiteralExpr;
class UnaryExpr;
class BinaryExpr;
class CallExpr;
class BindFrtExpr;
class IdentifierExpr;

class ForLoopStatement;
class WhileLoopStatement;
class ConditionalStatement;
class ElseStatement;
class RetStatement;
class CallStatement;
class AssignStatement;

class Type;
class VarType;
class FuncType;
class Block;

class ParserVisitor {
public:
    virtual ~ParserVisitor() = default;

    virtual void visit(const LiteralExpr& expr) = 0;
    virtual void visit(const IdentifierExpr& expr) = 0;
    virtual void visit(const UnaryExpr& expr) = 0;
    virtual void visit(const BinaryExpr& expr) = 0;
    virtual void visit(const CallExpr& expr) = 0;
    virtual void visit(const BindFrtExpr& expr) = 0;

    virtual void visit(const ForLoopStatement& stmt) = 0;
    virtual void visit(const WhileLoopStatement& stmt) = 0;
    virtual void visit(const ConditionalStatement& stmt) = 0;
    virtual void visit(const ElseStatement& stmt) = 0;
    virtual void visit(const RetStatement& stmt) = 0;
    virtual void visit(const CallStatement& stmt) = 0;
    virtual void visit(const AssignStatement& stmt) = 0;

    virtual void visit(const Block& block) = 0;
    virtual void visit(const VarType& var) = 0;
    virtual void visit(const FuncType& func) = 0;
};

class ParserPrinter : public ParserVisitor {
    private:
        std::ostream& os;
        int indent;

        std::string indent_str() const;
        void increase_indent();
        void decrease_indent();
    public:
        ParserPrinter(std::ostream& os, int indent = 0) : os(os), indent(indent) {}
        ~ParserPrinter() = default;

        void visit(const LiteralExpr& expr) override;
        void visit(const IdentifierExpr& expr) override;
        void visit(const UnaryExpr& expr) override;
        void visit(const BinaryExpr& expr) override;
        void visit(const CallExpr& expr) override;
        void visit(const BindFrtExpr& expr) override;

        void visit(const ForLoopStatement& stmt) override;
        void visit(const WhileLoopStatement& stmt) override;
        void visit(const ConditionalStatement& stmt) override;
        void visit(const ElseStatement& stmt) override;
        void visit(const RetStatement& stmt) override;
        void visit(const CallStatement& stmt) override;
        void visit(const AssignStatement& stmt) override;

        void visit(const Block& block) override;
        void visit(const VarType& type) override;
        void visit(const FuncType& type) override;
};
