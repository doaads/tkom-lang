#include "statement.h"
#include "expression.h"
#include "block.h"
#include "parser_visitor.h"
#include "type.h"
#include "tokens.h"

std::string Statement::indent_str(int amount) {
    return std::string(amount * 2, ' ');
}

/* ------------------------------[FOR LOOP]--------------------------------*/

ForLoopStatement::ForLoopStatement(
        std::unique_ptr<ForLoopArgs> args,
        std::unique_ptr<Block> body,
        std::unique_ptr<Expression> on_iter_call) :
    args(std::move(args)),
    body(std::move(body)),
    on_iter_call(std::move(on_iter_call)) {}

void ForLoopStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

const ForLoopArgs* ForLoopStatement::get_args() const {return args.get();}
const Block* ForLoopStatement::get_body() const {return body.get();}
const Expression* ForLoopStatement::get_on_iter() const {
    return on_iter_call.get();
}

/* -----------------------------[WHILE LOOP]-------------------------------*/

WhileLoopStatement::WhileLoopStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Block> body) :
    condition(std::move(condition)),
    body(std::move(body)) {}

void WhileLoopStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

const Expression* WhileLoopStatement::get_condition() const {
    return condition.get();
}
const Block* WhileLoopStatement::get_body() const {
    return body.get();
};

/* -----------------------------[CONDITIONAL]-------------------------------*/

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

void ConditionalStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

TokenType ConditionalStatement::get_type() const {return type;}

const Expression* ConditionalStatement::get_condition() const {
    return condition.get();
}

const Block* ConditionalStatement::get_body() const {
    return body.get();
}

const Statement* ConditionalStatement::get_else_st() const {
    return else_st.get();
}

/* -------------------------------[ELSE]---------------------------------*/

ElseStatement::ElseStatement(
        std::unique_ptr<Block> body) :
    body(std::move(body)) {}

void ElseStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

const Block* ElseStatement::get_body() const {
    return body.get();
}

/* -------------------------------[RET]----------------------------------*/

RetStatement::RetStatement(
        std::unique_ptr<Expression> retval) :
    retval(std::move(retval)) {}

void RetStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

const Expression* RetStatement::get_retval() const {
    return retval.get();
}

/* -------------------------------[CALL]---------------------------------*/

CallStatement::CallStatement(std::unique_ptr<Expression> call) :
    call(std::move(call)) {}

void CallStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

const Expression* CallStatement::get_call() const {
    return call.get();
}

/* ------------------------------[ASSIGN]--------------------------------*/

AssignStatement::AssignStatement(
        std::unique_ptr<Expression> value,
        std::unique_ptr<Type> type,
        std::string identifier) :
    value(std::move(value)),
    type(std::move(type)),
    identifier(std::make_unique<IdentifierExpr>(identifier)) {}

void AssignStatement::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

const Expression* AssignStatement::get_value() const {return value.get();}
const Type* AssignStatement::get_type() const {return type.get();}
const Expression* AssignStatement::get_identifier() const {return identifier.get();}

