#pragma once

#include "expression.h"
#include "tokens.h"
#include "type.h"
#include "block.h"
#include <memory>

class Block;
class Assign;
struct ForLoopArgs;

using ExprPtr = std::unique_ptr<Expression>;
using BlockPtr = std::unique_ptr<Block>;

class Statement {
    public:
        virtual ~Statement() = default;
};

class ForLoopStatement : public Statement {
    private:
        std::unique_ptr<ForLoopArgs> args;
        std::unique_ptr<Block> body;
        std::string on_iter_call;
    public:
        ForLoopStatement(
                std::unique_ptr<ForLoopArgs> args,
                std::unique_ptr<Block> body,
                std::string on_iter_call);
};


class WhileLoopStatement : public Statement {
    private:
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> body;
    public:
        WhileLoopStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> body);
};

class ConditionalStatement : public Statement {
    private:
        TokenType type;
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> body;
        std::unique_ptr<Statement> else_st;
    public:
        ConditionalStatement(
                TokenType type,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<Block> body,
                std::unique_ptr<Statement> else_st);
        ConditionalStatement(
                TokenType type,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<Block> body);
};

class ElseStatement : public Statement {
    private:
        std::unique_ptr<Block> body;
    public:
        ElseStatement(std::unique_ptr<Block> body);
};

class RetStatement : public Statement {
    private:
        std::unique_ptr<Expression> retval;
    public:
        RetStatement(std::unique_ptr<Expression> retval);
};

class CallStatement : public Statement {
    private:
        std::unique_ptr<CallExpr> call;
    public:
        CallStatement(std::unique_ptr<CallExpr> call);
};

class AssignStatement : public Statement {
    private:
        std::unique_ptr<Expression> value;
        std::unique_ptr<Type> type;
        std::string identifier;
    public:
        AssignStatement(
                std::unique_ptr<Expression> value,
                std::unique_ptr<Type> type,
                std::string identifier);
};

struct ForLoopArgs {
    std::variant<std::monostate, std::unique_ptr<Statement>, std::string> iterator;  // identifier or Assign
    std::unique_ptr<Expression> condition;
};
