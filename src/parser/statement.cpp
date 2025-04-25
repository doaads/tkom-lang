#include "statement.h"

ForLoopStatement::ForLoopStatement(
        std::unique_ptr<ForLoopArgs> args,
        std::unique_ptr<Block> body,
        std::string on_iter_call) :
    args(std::move(args)),
    body(std::move(body)),
    on_iter_call(on_iter_call) {}

WhileLoopStatement::WhileLoopStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Block> body) :
    condition(std::move(condition)),
    body(std::move(body)) {}

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

ElseStatement::ElseStatement(
        std::unique_ptr<Block> body) :
    body(std::move(body)) {}

RetStatement::RetStatement(
        std::unique_ptr<Expression> retval) :
    retval(std::move(retval)) {}

CallStatement::CallStatement(std::unique_ptr<CallExpr> call) :
    call(std::move(call)) {}

AssignStatement::AssignStatement(
        std::unique_ptr<Expression> value,
        std::unique_ptr<Type> type,
        std::string identifier) :
    value(std::move(value)),
    type(std::move(type)),
    identifier(identifier) {}
