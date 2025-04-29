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
    protected:
        static std::string indent_str(int amount);
    public:
        virtual void print(std::ostream& os, int indent = 0) const = 0;
        virtual ~Statement() = default;
        friend std::ostream& operator<<(std::ostream& os, const Statement& op);
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
        void print(std::ostream& os, int indent) const override;
};


class WhileLoopStatement : public Statement {
    private:
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> body;
    public:
        WhileLoopStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> body);
        void print(std::ostream& os, int indent) const override;
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
        void print(std::ostream& os, int indent) const override;
};

class ElseStatement : public Statement {
    private:
        std::unique_ptr<Block> body;
    public:
        ElseStatement(std::unique_ptr<Block> body);
        void print(std::ostream& os, int indent) const override;
};

class RetStatement : public Statement {
    private:
        std::unique_ptr<Expression> retval;
    public:
        RetStatement(std::unique_ptr<Expression> retval);
        void print(std::ostream& os, int indent) const override;
};

class CallStatement : public Statement {
    private:
        std::unique_ptr<Expression> call;
    public:
        CallStatement(std::unique_ptr<Expression> call);
        void print(std::ostream& os, int indent) const override;
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
        void print(std::ostream& os, int indent) const override;
};

struct ForLoopArgs {
    std::variant<std::monostate, std::unique_ptr<Statement>, std::string> iterator;  // identifier or Assign
    std::unique_ptr<Expression> condition;
    friend std::ostream& operator<<(std::ostream& os, ForLoopArgs args);
};
