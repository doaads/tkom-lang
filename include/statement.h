#pragma once

#include "expression.h"
#include "tokens.h"
#include <memory>

class Block;
class Assign;

class Statement {
    public:
        virtual ~Statement() = default;
};

class ForLoopStatement : public Statement {
    private:
        std::unique_ptr<Assign> iterator;
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> body;
        std::unique_ptr<CallExpr> on_iter_call;
    public:
        ForLoopStatement(std::unique_ptr<Assign> iterator, std::unique_ptr<Expression> condition, std::unique_ptr<Block> body, std::unique_ptr<CallExpr> on_iter_call);
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
        std::variant<std::monostate, std::unique_ptr<ConditionalStatement>> else_st;
    public:
        ConditionalStatement(TokenType type, std::unique_ptr<Expression> condition, std::unique_ptr<Block> body, std::unique_ptr<Statement> else_st);
        ConditionalStatement(TokenType type, std::unique_ptr<Expression> condition, std::unique_ptr<Block> body);
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
