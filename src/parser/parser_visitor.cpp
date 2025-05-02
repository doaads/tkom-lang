#include "parser_visitor.h"

std::string ParserPrinter::indent_str() const {
    return std::string(indent * 2, ' ');
}

void ParserPrinter::increase_indent() { ++indent; }
void ParserPrinter::decrease_indent() { --indent; }

void ParserPrinter::visit(const LiteralExpr& expr) {
    os << indent_str() << "LiteralExpr: ";
    std::variant<const Expression*, Token> val = expr.get_value();
    if (val.index()) {
        os << std::get<Token>(val) << std::endl;
    } else {
        increase_indent();
        std::get<const Expression*>(val)->accept(*this);
        decrease_indent();
    }
}

void ParserPrinter::visit(const BinaryExpr& expr) {
    os << indent_str() << "BinaryExpr: " << expr.get_operator() << "\n";
    increase_indent();
    expr.get_left()->accept(*this);
    expr.get_right()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const UnaryExpr& expr) {
    os << indent_str() << "UnaryExpr: " << expr.get_operator() << "\n";
    increase_indent();
    expr.get_right()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const CallExpr& expr) {
    os << indent_str() << "CallExpr: " << std::endl;
    increase_indent();
    os << indent_str() << "Func:" << std::endl;
    increase_indent();
    expr.get_func_name()->accept(*this);
    decrease_indent();
    os << indent_str() << "Args:" << std::endl;
    std::vector<const Expression*> args = expr.get_args();
    increase_indent();
    if (args.empty()) {
        os << indent_str() << "(none)\n";
    } else {
        for (const auto& arg : args) {
            if (arg) {
                arg->accept(*this);
            } else {
                os << "null\n";
            }
        }
    }
    decrease_indent();
}

void ParserPrinter::visit(const BindFrtExpr& expr) {
    os << indent_str() << "BindFrtExpr: " << std::endl;
    increase_indent();
    expr.get_func_name()->accept(*this);
    std::vector<const Expression*> args = expr.get_args();
    increase_indent();
    if (args.empty()) {
        os << indent_str() << "(none)\n";
    } else {
        for (const auto& arg : args) {
            if (arg) {
                arg->accept(*this);
            } else {
                os << "null\n";
            }
        }
    }
    decrease_indent();
}
