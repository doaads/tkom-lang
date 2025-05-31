#include "visitor.h"

#include "block.h"
#include "expression.h"
#include "function.h"
#include "statement_specific.h"
#include "type.h"

auto ParserPrinter::indent_str() const -> std::string {
    return (std::string("┃") + std::string(indent * 2, ' '));
}

void ParserPrinter::increase_indent() { ++indent; }
void ParserPrinter::decrease_indent() { --indent; }

void ParserPrinter::print_pos(const Node &expr) {
    os << " at (\033[1;36m" << expr.get_position() << "\033[0m)";
}

void ParserPrinter::visit(const Program &program) {
    for (const auto &func : program.get_functions()) {
        func->accept(*this);
    }
}

void ParserPrinter::visit(const LiteralExpr &expr) {
    os << indent_str() << "└[LiteralExpr \033[1m";
    os << expr.get_value_string() << "\033[0m] ";
    os << "at ";
    print_pos(expr);
    os << std::endl;
}

void ParserPrinter::visit(const IdentifierExpr &expr) {
    os << indent_str() << "└[Identifier \033[1m";
    os << expr.get_identifier() << "\033[0m]";
    print_pos(expr);
    os << std::endl;
}

void ParserPrinter::visit(const BinaryExpr &expr) {
    os << indent_str() << "└[\033[1;32mBinaryExpr " << expr.get_operator() << "\033[0m]";
    print_pos(expr);
    os << std::endl;
    increase_indent();
    expr.get_left()->accept(*this);
    expr.get_right()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const UnaryExpr &expr) {
    os << indent_str() << "└[\033[1;32mUnaryExpr\033[0m]" << expr.get_operator();
    print_pos(expr);
    os << std::endl;
    increase_indent();
    expr.get_right()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const CallExpr &expr) {
    os << indent_str() << "└[\033[1;32mCallExpr\033[0m]";
    print_pos(expr);
    os << std::endl;
    increase_indent();
    expr.get_func_name()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const BindFrtExpr &expr) {
    os << indent_str() << "└[\033[1;32m[BindFrtExpr\033[0m]";
    print_pos(expr);
    os << std::endl;
    expr.get_func_name()->accept(*this);
}

void ParserPrinter::visit(const ForLoopStatement &stmt) {
    os << indent_str() << "[\033[1;36mForLoop\033[0m]";
    print_pos(stmt);
    os << std::endl;
    stmt.get_on_iter()->accept(*this);
    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const WhileLoopStatement &stmt) {
    os << indent_str() << "[\033[1;36mWhileLoop\033[0m]";
    print_pos(stmt);
    os << std::endl;

    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const ConditionalStatement &stmt) {
    os << indent_str() << "[\033[1;36mConditional\033[0m " << stmt.get_type() << "]";
    print_pos(stmt);
    os << std::endl;
    stmt.get_condition()->accept(*this);

    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
    if (const Statement *else_st = stmt.get_else_st()) else_st->accept(*this);
}
void ParserPrinter::visit(const ElseStatement &stmt) {
    os << indent_str() << "[\033[1;36mElse\033[0m]";
    print_pos(stmt);
    os << std::endl;

    increase_indent();
    stmt.get_body()->accept(*this);
    decrease_indent();
}
void ParserPrinter::visit(const RetStatement &stmt) {
    os << indent_str() << "[\033[1;36mRet\033[0m]";
    print_pos(stmt);
    os << std::endl;
    if (auto retval = stmt.get_retval()) retval->accept(*this);
}

void ParserPrinter::visit(const CallStatement &stmt) { stmt.get_call()->accept(*this); }

void ParserPrinter::visit(const AssignStatement &stmt) {
    os << indent_str() << "[\033[1;36mAssign\033[0m] ";
    os << "\033[1;31m";
    if (auto type = stmt.get_type()) type->accept(*this);
    os << "\033[0m";
    os << stmt.get_identifier() << " ";
    print_pos(stmt);
    os << std::endl;
    increase_indent();
    stmt.get_value()->accept(*this);
    decrease_indent();
}

void ParserPrinter::visit(const VarType &type) {
    os << (type.get_mut() ? "mut " : "");
    os << type.get_type();
}

void ParserPrinter::visit(const Block &block) {
    increase_indent();
    for (auto &stmt : block.get_statements()) {
        stmt->accept(*this);
    }
    decrease_indent();
}

void ParserPrinter::visit(const FuncType &type) {
    os << "[";
    const Type *ret_type = type.get_ret_type();
    if (!ret_type) {
        os << "void";
    } else {
        ret_type->accept(*this);
    }

    os << "::";
    bool first_done = false;
    for (auto &param : type.get_params()) {
        if (first_done) {
            os << ",";
        } else {
            first_done = true;
        }
        param->accept(*this);
    }
    os << "]";
}

void ParserPrinter::visit(const VariableSignature &var) { var.get_type()->accept(*this); }

void ParserPrinter::visit(const FuncSignature &sign) {
    os << "┏━━━FUNCTION: \033[1;33m";
    os << sign.get_name() << "\033[0m";
    print_pos(sign);
    os << std::endl;
    os << "┃ TYPE: ";
    const Type *type = sign.get_type();
    if (type) {
        os << "\033[1;31m";
        type->accept(*this);
        os << "\033[0m";
    }
    os << std::endl;
    os << "┃ PARAMS: ";
    bool first = true;
    for (auto &param : sign.get_params()) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        param->accept(*this);
    }
    os << std::endl;
}

void ParserPrinter::visit(const Function &func) {
    func.get_signature()->accept(*this);
    os << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
    os << std::endl;
    increase_indent();
    func.get_body()->accept(*this);
    decrease_indent();
    os << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
    os << std::endl;
}
