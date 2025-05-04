#include "expression.h"
#include "parser_visitor.h"
#include "type.h"

#include <string>

/* ------------------------------[LITERAL]--------------------------------*/

LiteralExpr::LiteralExpr(Token token) :
    value(token.get_value()) {
        std::optional<BaseType> type = get_literal_token_type(token.get_type());
        if (!type) throw std::runtime_error("Invalid type passed on to LiteralExpr");
        LiteralExpr::type = *type;
    }

void LiteralExpr::accept(ParserVisitor& visitor) const {
    visitor.visit(*this);
}

std::string LiteralExpr::get_value_string() const {
    return std::visit([](auto&& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "null";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return val;
        } else if constexpr (std::is_same_v<T, bool>) {
            return val ? "true" : "false";
        } else if constexpr (std::is_arithmetic_v<T>){
            return std::to_string(val);
        } else {
            throw std::runtime_error("Invalid type");
        }
    }, value);
}

/* ----------------------------[IDENTIFIER]--------------------------------*/

IdentifierExpr::IdentifierExpr(std::string identifier) : identifier(identifier) {}

std::string IdentifierExpr::get_identifier() const {return identifier;}
void IdentifierExpr::accept(ParserVisitor& visitor) const {visitor.visit(*this);}


/* ------------------------------[UNARY]--------------------------------*/

UnaryExpr::UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right) :
    op_type(unary_op), right(std::move(right)) {}

UnaryOp UnaryExpr::get_operator() const {
    return op_type;
}

void UnaryExpr::accept(ParserVisitor& visitor) const {
    visitor.visit(*this);
}

const Expression* UnaryExpr::get_right() const {
    return right.get();
}

/* ------------------------------[BINARY]--------------------------------*/

BinaryExpr::BinaryExpr(
        std::unique_ptr<Expression> left,
        BinaryOp op,
        std::unique_ptr<Expression> right) :
    left(std::move(left)), op(op), right(std::move(right)) {}

void BinaryExpr::accept(ParserVisitor& visitor) const {
    visitor.visit(*this);
}

BinaryOp BinaryExpr::get_operator() const {
    return op;
}

const Expression* BinaryExpr::get_left() const {
    return left.get();
}

const Expression* BinaryExpr::get_right() const {
    return right.get();
}

/* ------------------------------[CALL]--------------------------------*/

CallExpr::CallExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args) : func_name(std::move(name)), args(std::move(args)) {}

const Expression* CallExpr::get_func_name() const {
    return func_name.get();
}

const std::vector<const Expression*> CallExpr::get_args() const {
    std::vector<const Expression*> result;
    result.reserve(args.size());
    for (const auto& arg : args) {
        result.push_back(arg.get());
    }
    return result;
}

void CallExpr::accept(ParserVisitor& visitor) const {
    visitor.visit(*this);
}

/* ------------------------------[BINDFRT]--------------------------------*/

BindFrtExpr::BindFrtExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args) : func_name(std::move(name)), args(std::move(args)) {}

void BindFrtExpr::accept(ParserVisitor& visitor) const {
    visitor.visit(*this);
}

const Expression* BindFrtExpr::get_func_name() const {
    return func_name.get();
}

const std::vector<const Expression*> BindFrtExpr::get_args() const {
    std::vector<const Expression*> result;
    result.reserve(args.size());
    for (const auto& arg : args) {
        result.push_back(arg.get());
    }
    return result;
}
