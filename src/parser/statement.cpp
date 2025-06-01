#include "block.h"
#include "expression.h"
#include "visitor.h"
#include "statement_specific.h"
#include "tokens.h"
#include "type.h"

auto Statement::indent_str(int amount) -> std::string { return std::string(amount * 2, ' '); }

/* ------------------------------[FOR LOOP]--------------------------------*/

ForLoopStatement::ForLoopStatement(Position pos, std::unique_ptr<ForLoopArgs> args,
                                   std::unique_ptr<Block> body,
                                   std::unique_ptr<Expression> on_iter_call)
    : Statement(pos),
      args(std::move(args)),
      body(std::move(body)),
      on_iter_call(std::move(on_iter_call)) {}

void ForLoopStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

auto ForLoopStatement::get_args() const -> const ForLoopArgs * { return args.get(); }
auto ForLoopStatement::get_body() const -> const Block * { return body.get(); }
auto ForLoopStatement::get_on_iter() const -> const Expression * { return on_iter_call.get(); }

/* -----------------------------[WHILE LOOP]-------------------------------*/

WhileLoopStatement::WhileLoopStatement(Position pos, std::unique_ptr<Expression> condition,
                                       std::unique_ptr<Block> body)
    : Statement(pos), condition(std::move(condition)), body(std::move(body)) {}

void WhileLoopStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

auto WhileLoopStatement::get_condition() const -> const Expression * { return condition.get(); }
auto WhileLoopStatement::get_body() const -> const Block * { return body.get(); };

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

auto ConditionalStatement::get_type() const -> TokenType { return type; }

auto ConditionalStatement::get_condition() const -> const Expression * { return condition.get(); }

auto ConditionalStatement::get_body() const -> const Block * { return body.get(); }

auto ConditionalStatement::get_else_st() const -> const Statement * { return else_st.get(); }

/* -------------------------------[ELSE]---------------------------------*/

ElseStatement::ElseStatement(Position pos, std::unique_ptr<Block> body)
    : Statement(pos), body(std::move(body)) {}

void ElseStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

auto ElseStatement::get_body() const -> const Block * { return body.get(); }

/* -------------------------------[RET]----------------------------------*/

RetStatement::RetStatement(Position pos, std::unique_ptr<Expression> retval)
    : Statement(pos), retval(std::move(retval)) {}

void RetStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

auto RetStatement::get_retval() const -> const Expression * { return retval.get(); }

/* -------------------------------[CALL]---------------------------------*/

CallStatement::CallStatement(Position pos, std::unique_ptr<Expression> call)
    : Statement(pos), call(std::move(call)) {}

void CallStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

auto CallStatement::get_call() const -> const Expression * { return call.get(); }

/* ------------------------------[ASSIGN]--------------------------------*/

AssignStatement::AssignStatement(Position pos, std::unique_ptr<Expression> value,
                                 std::unique_ptr<VariableSignature> signature)
    : Statement(pos),
      value(std::move(value)),
      var_sign(std::move(signature)) {}

void AssignStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

auto AssignStatement::get_value() const -> const Expression * { return value.get(); }
auto AssignStatement::get_type() const -> const Type * { return var_sign->get_type(); }
auto AssignStatement::get_identifier() const -> const std::string { return var_sign->get_name(); }
auto AssignStatement::get_signature() const -> const VariableSignature * { return var_sign.get(); }
