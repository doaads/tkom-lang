#pragma once

/**
 * @file statement_specific.h
 *
 * @brief Contains specific statement classes representing various control flow and function-related
 * constructs.
 *
 * This file defines concrete subclasses of `Statement` that represent specific control flow
 * structures such as for-loops, while-loops, conditionals, return statements, and function calls.
 */

#include "block.h"
#include "statement.h"
#include "variable.h"

class Expression;
class Visitor;
class Type;
enum class TokenType;

/**
 * @brief Represents arguments passed to a `for` loop, such as initializer, condition, and iterator
 * expression.
 */
struct ForLoopArgs {
    std::variant<std::unique_ptr<Statement>, std::string>
        iterator;  // identifier or Assign
    std::unique_ptr<Expression> condition;
    friend std::ostream &operator<<(std::ostream &os, ForLoopArgs args);
};

/**
 * @brief Represents a `for` loop statement in the AST.
 */
class ForLoopStatement : public Statement {
   private:
    std::unique_ptr<ForLoopArgs> args;
    std::unique_ptr<Block> body;
    std::unique_ptr<Expression> on_iter_call;

   public:
    ForLoopStatement(Position pos, std::unique_ptr<ForLoopArgs> args, std::unique_ptr<Block> body,
                     std::unique_ptr<Expression> on_iter_call);

    void accept(Visitor &visitor) const override;

    const ForLoopArgs *get_args() const;
    const Block *get_body() const;
    const Expression *get_on_iter() const;
};

/**
 * @brief Represents a conditional `if` or `elif` statement in the AST.
 */
class WhileLoopStatement : public Statement {
   private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> body;

   public:
    WhileLoopStatement(Position pos, std::unique_ptr<Expression> condition,
                       std::unique_ptr<Block> body);

    void accept(Visitor &visitor) const override;

    const Expression *get_condition() const;
    const Block *get_body() const;
};

/**
 * @brief Represents an `else` statement block.
 */
class ConditionalStatement : public Statement {
   private:
    TokenType type;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> body;
    std::unique_ptr<Statement> else_st;

   public:
    ConditionalStatement(Position pos, TokenType type, std::unique_ptr<Expression> condition,
                         std::unique_ptr<Block> body, std::unique_ptr<Statement> else_st);
    ConditionalStatement(Position pos, TokenType type, std::unique_ptr<Expression> condition,
                         std::unique_ptr<Block> body);

    void accept(Visitor &visitor) const override;

    TokenType get_type() const;
    const Expression *get_condition() const;
    const Block *get_body() const;
    const Statement *get_else_st() const;
};

/**
 * @brief Represents a `return` statement in a function.
 */
class ElseStatement : public Statement {
   private:
    std::unique_ptr<Block> body;

   public:
    ElseStatement(Position pos, std::unique_ptr<Block> body);

    void accept(Visitor &visitor) const override;

    const Block *get_body() const;
};

/**
 * @brief Represents a standalone expression call as a statement.
 */
class RetStatement : public Statement {
   private:
    std::unique_ptr<Expression> retval;

   public:
    RetStatement(Position pos, std::unique_ptr<Expression> retval);

    void accept(Visitor &visitor) const override;

    const Expression *get_retval() const;
};

/**
 * @brief Represents a standalone expression as a statement.
 */
class CallStatement : public Statement {
   private:
    std::unique_ptr<Expression> call;

   public:
    CallStatement(Position pos, std::unique_ptr<Expression> call);

    void accept(Visitor &visitor) const override;

    const Expression *get_call() const;
};

/**
 * @brief Represents a variable assignment statement, optionally with type annotation.
 */
class AssignStatement : public Statement {
   private:
    std::unique_ptr<Expression> value;
    std::unique_ptr<VariableSignature> var_sign;

   public:
    AssignStatement(Position pos, std::unique_ptr<Expression> value, std::unique_ptr<VariableSignature> sign);

    void accept(Visitor &visitor) const override;

    const Expression *get_value() const;
    const Type *get_type() const;
    const std::string get_identifier() const;
    const VariableSignature* get_signature() const;
};
