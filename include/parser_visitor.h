#pragma once

#include <ostream>
#include "expression.h"

class LiteralExpr;
class UnaryExpr;
class BinaryExpr;
class CallExpr;
class BindFrtExpr;

class ParserVisitor {
public:
    virtual ~ParserVisitor() = default;

    virtual void visit(const LiteralExpr& expr) = 0;
    virtual void visit(const UnaryExpr& expr) = 0;
    virtual void visit(const BinaryExpr& expr) = 0;
    virtual void visit(const CallExpr& expr) = 0;
    virtual void visit(const BindFrtExpr& expr) = 0;
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
        void visit(const UnaryExpr& expr) override;
        void visit(const BinaryExpr& expr) override;
        void visit(const CallExpr& expr) override;
        void visit(const BindFrtExpr& expr) override;
};
