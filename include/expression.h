#pragma once
/**
 * @file expression.h
 * 
 * @brief Contains the definitions of various expression types in the program tree.
 * 
 * This file defines classes for different types of expressions used in the language, namely: literal
 * expressions, identifier expressions, unary and binary expressions, function calls, and function
 * bind front expressions.
 */


#include <memory>
#include <vector>
#include <variant>

#include "node.h"
#include "token.h"
#include "operators.h"

enum class BaseType;
class ParserVisitor;

/**
 * @brief Abstract base class for all expression types in the program tree.
 */
class Expression : public Node {
    public:
        /**
         * @brief Constructor for the Expression class.
         * 
         * @param pos The position of the expression in the source code.
         */
        Expression(const Position pos) : Node(pos) {}
        virtual void accept(ParserVisitor& visitor) const = 0;
};


using ValueType = std::variant<std::monostate, std::string, int, double, bool>;

/**
 * @brief Represents a literal value (string, int, double, bool) in the AST.
 * 
 * Used to represent literal values in the source code, such as numbers, strings, and booleans.
 */
class LiteralExpr : public Expression {
    private:
        BaseType type;
        ValueType value;
        Position position;
    public:
        template<typename T>
        T get_value() const {
            if (const T* pval = std::get_if<T>(value)) {
                return *pval;
            } else {
                throw std::runtime_error("Unable to get LiteralExpr value");
            }
        }
        LiteralExpr(const Position pos, Token token);
        void accept(ParserVisitor& visitor) const override;
        std::string get_value_string() const;
};

/**
 * @brief Represents an identifier (e.g., variable or function name).
 */
class IdentifierExpr : public Expression {
    private:
        std::string identifier;
    public:
        IdentifierExpr(const Position pos, std::string identifier);

        void accept(ParserVisitor& visitor) const override;
        std::string get_identifier() const;
};

/**
 * @brief Represents a unary expression (e.g., negation or logical NOT).
 * 
 * The `UnaryExpr` class is used to represent unary operations, such as negation (`!`) or logical
 * NOT. It stores the operator type and the right-hand operand (which is a sub-expression).
 */
class UnaryExpr : public Expression {
    private:
        UnaryOp op_type;
        std::unique_ptr<Expression> right;
    public:
        UnaryExpr(const Position pos, UnaryOp unary_op, std::unique_ptr<Expression> right);
        UnaryOp get_operator() const;
        void accept(ParserVisitor& visitor) const;

        const Expression* get_right() const;
};

/**
 * @brief Represents a binary expression (e.g., addition, subtraction, etc.).
 * 
 * The `BinaryExpr` class is used to represent binary operations, namely: addition, subtraction,
 * multiplication, division, comparison, decoration and logical AND and OR. It stores the left and right operands along with the operator.
 */
class BinaryExpr : public Expression {
    private:
        std::unique_ptr<Expression> left;
        BinaryOp op;
        std::unique_ptr<Expression> right;
    public:
        BinaryExpr(const Position pos, std::unique_ptr<Expression> left, BinaryOp op, std::unique_ptr<Expression> right);
        void accept(ParserVisitor& visitor) const;

        const Expression* get_left() const;
        BinaryOp get_operator() const;
        const Expression* get_right() const;
};


/**
 * @brief Represents a function call expression.
 * 
 * The `CallExpr` class is used to represent function calls in the source code. It stores the
 * function name (which is an expression) and the list of arguments (which are also expressions).
 */
class CallExpr : public Expression {
    private:
        std::unique_ptr<Expression> func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        CallExpr(const Position pos, std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args);
        void accept(ParserVisitor& visitor) const;

        const Expression* get_func_name() const;
        const std::vector<const Expression*> get_args() const;
};


/**
 * @brief Represents a bind front-right expression (used in specific language features).
 */
class BindFrtExpr : public Expression {
    private:
        std::unique_ptr<Expression> func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        BindFrtExpr(const Position pos, std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args);
        void accept(ParserVisitor& visitor) const;

        const Expression* get_func_name() const;
        const std::vector<const Expression*> get_args() const;
};
