#include "parser_visitor.h"
#include "type.h"
#include "statement.h"
#include "expression.h"
#include "block.h"

std::string ParserPrinter::indent_str() const {
    return std::string(indent * 2, ' ');
}

void ParserPrinter::increase_indent() { ++indent; }
void ParserPrinter::decrease_indent() { --indent; }

void ParserPrinter::visit(const LiteralExpr& expr) {
    os << indent_str() << "└[LiteralExpr \033[1m";
    os << expr.get_value_string() << "\033[0m]" << std::endl;
}

void ParserPrinter::visit(const IdentifierExpr& expr) {
    os << indent_str() << "└[Identifier \033[1m";
    os << expr.get_identifier() << "\033[0m]" << std::endl;
}

void ParserPrinter::visit(const BinaryExpr& expr) {
    os << indent_str() << "└[\033[1;32mBinaryExpr " << expr.get_operator() << "\033[0m]\n";
    increase_indent();
    expr.get_left()->accept(*this);
    expr.get_right()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const UnaryExpr& expr) {
    os << indent_str() << "└[\033[1;32mUnaryExpr\033[0m] " << expr.get_operator() << "\n";
    increase_indent();
    expr.get_right()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const CallExpr& expr) {
    os << indent_str() << "└[\033[1;32mCallExpr\033[0m] " << std::endl;
    increase_indent();
    expr.get_func_name()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const BindFrtExpr& expr) {
    os << indent_str() << "└[\033[1;32m[BindFrtExpr\033[0m] " << std::endl;
    expr.get_func_name()->accept(*this);
}


void ParserPrinter::visit(const ForLoopStatement& stmt) {
    os << indent_str() << "[\033[1;36mForLoop\033[0m]" << std::endl;
    stmt.get_on_iter()->accept(*this);
    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const WhileLoopStatement& stmt) {
    os << indent_str() << "[\033[1;36mWhileLoop\033[0m] ";
    os << std::endl;

    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const ConditionalStatement& stmt) {
    os << indent_str() << "[\033[1;36mConditional\033[0m] " << stmt.get_type() << std::endl;

    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}
void ParserPrinter::visit(const ElseStatement& stmt) {
    os << indent_str() << "[\033[1;36mElse\033[0m] " << std::endl;

    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}
void ParserPrinter::visit(const RetStatement& stmt) {
    os << indent_str() << "[\033[1;36mRet\033[0m] " << stmt.get_retval() << std::endl;
}

void ParserPrinter::visit(const CallStatement& stmt) {
    stmt.get_call()->accept(*this);
}
void ParserPrinter::visit(const AssignStatement& stmt) {
    os << indent_str() << "[\033[1;36mAssign\033[0m] ";
    os << "\033[1;31m";
    stmt.get_type()->accept(*this);
    os << "\033[0m" << std::endl;
    stmt.get_identifier()->accept(*this);
    increase_indent();
    stmt.get_value()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const VarType& type) {
    os << (type.get_mut() ? "mut " : "");
    os << type.get_type();
}

void ParserPrinter::visit(const Block& block) {
    increase_indent();
    for (auto& stmt : block.get_statements()) {
        stmt->accept(*this);
    }
    decrease_indent();
}

void ParserPrinter::visit(const FuncType& type) {
    os << "[";
    const Type* ret_type = type.get_ret_type();
    if (!ret_type) {
        os << "void";
    } else {
        ret_type->accept(*this);
    }

    os << "::";
    bool first_done = false;
    for (auto& param : type.get_params()) {
        if (first_done) {
            os << ",";
        } else {
            first_done = true;
        }
        param->accept(*this);
    }
    os << "]";
}

