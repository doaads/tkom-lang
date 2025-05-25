#include "block.h"
#include "expression.h"
#include "visitor.h"
#include "statement_specific.h"
#include "tokens.h"
#include "type.h"

std::string Statement::indent_str(int amount) { return std::string(amount * 2, ' '); }

/* ------------------------------[FOR LOOP]--------------------------------*/

ForLoopStatement::ForLoopStatement(Position pos, std::unique_ptr<ForLoopArgs> args,
                                   std::unique_ptr<Block> body,
                                   std::unique_ptr<Expression> on_iter_call)
    : Statement(pos),
      args(std::move(args)),
      body(std::move(body)),
      on_iter_call(std::move(on_iter_call)) {}

void ForLoopStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

const ForLoopArgs *ForLoopStatement::get_args() const { return args.get(); }
const Block *ForLoopStatement::get_body() const { return body.get(); }
const Expression *ForLoopStatement::get_on_iter() const { return on_iter_call.get(); }

/* -----------------------------[WHILE LOOP]-------------------------------*/

WhileLoopStatement::WhileLoopStatement(Position pos, std::unique_ptr<Expression> condition,
                                       std::unique_ptr<Block> body)
    : Statement(pos), condition(std::move(condition)), body(std::move(body)) {}

void WhileLoopStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

const Expression *WhileLoopStatement::get_condition() const { return condition.get(); }
const Block *WhileLoopStatement::get_body() const { return body.get(); };

/* -----------------------------[CONDITIONAL]-------------------------------*/

ConditionalStatement::ConditionalStatement(Position pos, TokenType type,
                                           std::unique_ptr<Expression> condition,
                                           std::unique_ptr<Block> body,
                                           std::unique_ptr<Statement> else_st)
    : Statement(pos),
      type(type),
      condition(std::move(condition)),
      body(std::move(body)),
      else_st(std::move(else_st)) {}

ConditionalStatement::ConditionalStatement(Position pos, TokenType type,
                                           std::unique_ptr<Expression> condition,
                                           std::unique_ptr<Block> body)
    : Statement(pos), type(type), condition(std::move(condition)), body(std::move(body)) {}

void ConditionalStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

TokenType ConditionalStatement::get_type() const { return type; }

const Expression *ConditionalStatement::get_condition() const { return condition.get(); }

const Block *ConditionalStatement::get_body() const { return body.get(); }

const Statement *ConditionalStatement::get_else_st() const { return else_st.get(); }

/* -------------------------------[ELSE]---------------------------------*/

ElseStatement::ElseStatement(Position pos, std::unique_ptr<Block> body)
    : Statement(pos), body(std::move(body)) {}

void ElseStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

const Block *ElseStatement::get_body() const { return body.get(); }

/* -------------------------------[RET]----------------------------------*/

RetStatement::RetStatement(Position pos, std::unique_ptr<Expression> retval)
    : Statement(pos), retval(std::move(retval)) {}

void RetStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

const Expression *RetStatement::get_retval() const { return retval.get(); }

/* -------------------------------[CALL]---------------------------------*/

CallStatement::CallStatement(Position pos, std::unique_ptr<Expression> call)
    : Statement(pos), call(std::move(call)) {}

void CallStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

const Expression *CallStatement::get_call() const { return call.get(); }

/* ------------------------------[ASSIGN]--------------------------------*/

AssignStatement::AssignStatement(Position pos, std::unique_ptr<Expression> value,
                                 std::unique_ptr<VariableSignature> signature)
    : Statement(pos),
      value(std::move(value)),
      var_sign(std::move(signature)) {}

void AssignStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

const Expression *AssignStatement::get_value() const { return value.get(); }
const Type *AssignStatement::get_type() const { return var_sign->get_type(); }
const Expression *AssignStatement::get_identifier() const { return var_sign->get_name(); }
