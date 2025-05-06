#pragma once

#include "expression.h"
#include "node.h"
#include <memory>
#include <string>
#include <variant>

class Expression;
class Block;
class ParserVisitor;
class Type;
struct ForLoopArgs;
enum class TokenType;

using ExprPtr = std::unique_ptr<Expression>;
using BlockPtr = std::unique_ptr<Block>;

class Statement : public Node {
    protected:
        static std::string indent_str(int amount);
    public:
        Statement(const Position pos) : Node(pos) {}
        virtual ~Statement() = default;

        virtual void accept(ParserVisitor& visitor) const = 0;
};

class ForLoopStatement : public Statement {
    private:
        std::unique_ptr<ForLoopArgs> args;
        std::unique_ptr<Block> body;
        std::unique_ptr<Expression> on_iter_call;
    public:
        ForLoopStatement(
                Position pos,
                std::unique_ptr<ForLoopArgs> args,
                std::unique_ptr<Block> body,
                std::unique_ptr<Expression> on_iter_call);

        void accept(ParserVisitor& visitor) const override;

        const ForLoopArgs* get_args() const;
        const Block* get_body() const;
        const Expression* get_on_iter() const;
};


class WhileLoopStatement : public Statement {
    private:
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> body;
    public:
        WhileLoopStatement(Position pos, std::unique_ptr<Expression> condition, std::unique_ptr<Block> body);

        void accept(ParserVisitor& visitor) const override;
        
        const Expression* get_condition() const;
        const Block* get_body() const;
};

class ConditionalStatement : public Statement {
    private:
        TokenType type;
        std::unique_ptr<Expression> condition;
        std::unique_ptr<Block> body;
        std::unique_ptr<Statement> else_st;
    public:
        ConditionalStatement(
                Position pos,
                TokenType type,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<Block> body,
                std::unique_ptr<Statement> else_st);
        ConditionalStatement(
                Position pos,
                TokenType type,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<Block> body);

        void accept(ParserVisitor& visitor) const override;

        TokenType get_type() const;
        const Expression* get_condition() const;
        const Block* get_body() const;
        const Statement* get_else_st() const;

};

class ElseStatement : public Statement {
    private:
        std::unique_ptr<Block> body;
    public:
        ElseStatement(Position pos, std::unique_ptr<Block> body);

        void accept(ParserVisitor& visitor) const override;

        const Block* get_body() const;
};

class RetStatement : public Statement {
    private:
        std::unique_ptr<Expression> retval;
    public:
        RetStatement(Position pos, std::unique_ptr<Expression> retval);

        void accept(ParserVisitor& visitor) const override;

        const Expression* get_retval() const;
};

class CallStatement : public Statement {
    private:
        std::unique_ptr<Expression> call;
    public:
        CallStatement(Position pos, std::unique_ptr<Expression> call);

        void accept(ParserVisitor& visitor) const override;

        const Expression* get_call() const;
};

class AssignStatement : public Statement {
    private:
        std::unique_ptr<Expression> value;
        std::unique_ptr<Type> type;
        std::unique_ptr<Expression> identifier;
    public:
        AssignStatement(
                Position pos,
                std::unique_ptr<Expression> value,
                std::unique_ptr<Type> type,
                std::unique_ptr<Expression>);

        void accept(ParserVisitor& visitor) const override;

        const Expression* get_value() const;
        const Type* get_type() const;
        const Expression* get_identifier() const;
};

struct ForLoopArgs {
    std::variant<std::monostate, std::unique_ptr<Statement>, std::unique_ptr<Expression>> iterator;  // identifier or Assign
    std::unique_ptr<Expression> condition;
    friend std::ostream& operator<<(std::ostream& os, ForLoopArgs args);
};
