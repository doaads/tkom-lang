#include "statement.h"

std::ostream& operator<<(std::ostream& os, const Statement& op) {
    op.print(os);
    return os;
}

std::string Statement::indent_str(int amount) {
    return std::string(amount * 2, ' ');
}

ForLoopStatement::ForLoopStatement(
        std::unique_ptr<ForLoopArgs> args,
        std::unique_ptr<Block> body,
        std::string on_iter_call) :
    args(std::move(args)),
    body(std::move(body)),
    on_iter_call(on_iter_call) {}

void ForLoopStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "ForLoopStatement: " << std::endl;
    // TODO: uncomment this after block is done
    //os << indent_str(indent + 1) << "body: ";
    //body->print(os, indent + 1);
    //os << std::endl;
    os << indent_str(indent + 1) << "after iter: " << on_iter_call << std::endl; 
}

WhileLoopStatement::WhileLoopStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Block> body) :
    condition(std::move(condition)),
    body(std::move(body)) {}

void WhileLoopStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "WhileLoopStatement: " << std::endl;
    os << indent_str(indent + 1) << "condition: ";
    condition->print(os, indent + 1);
    os << std::endl;
}

ConditionalStatement::ConditionalStatement(
        TokenType type,
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Block> body,
        std::unique_ptr<Statement> else_st) :
    type(type),
    condition(std::move(condition)),
    body(std::move(body)),
    else_st(std::move(else_st)) {}

ConditionalStatement::ConditionalStatement(
        TokenType type,
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Block> body) :
    type(type),
    condition(std::move(condition)),
    body(std::move(body)) {}

void ConditionalStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "ConditionalStatement: " << type << std::endl;
    os << indent_str(indent + 1) << "condition: ";
    condition->print(os, indent + 1);
    os << indent_str(indent) << "Else: " << std::endl;
    else_st->print(os, indent + 1);
    os << std::endl;
}

ElseStatement::ElseStatement(
        std::unique_ptr<Block> body) :
    body(std::move(body)) {}


void ElseStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "ElseStatement:" << std::endl;
}

RetStatement::RetStatement(
        std::unique_ptr<Expression> retval) :
    retval(std::move(retval)) {}

void RetStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "Ret:" << std::endl;
    retval->print(os, indent + 1);
}

CallStatement::CallStatement(std::unique_ptr<Expression> call) :
    call(std::move(call)) {}


void CallStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "Call:" << std::endl;
    call->print(os, indent + 1);
}

AssignStatement::AssignStatement(
        std::unique_ptr<Expression> value,
        std::unique_ptr<Type> type,
        std::string identifier) :
    value(std::move(value)),
    type(std::move(type)),
    identifier(identifier) {}

void AssignStatement::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "Assign: to " << identifier << std::endl;
    value->print(os, indent + 1);
}
