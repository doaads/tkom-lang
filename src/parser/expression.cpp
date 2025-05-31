#include "expression.h"

#include <string>
#include <utility>

#include "type.h"
#include "visitor.h"

/* ------------------------------[LITERAL]--------------------------------*/

LiteralExpr::LiteralExpr(const Position pos, Token token)
    : Expression(pos), value(token.get_value()) {
    std::optional<BaseType> type = get_literal_token_type(token.get_type());
    if (!type) throw std::runtime_error("Invalid type passed on to LiteralExpr");
    LiteralExpr::type = *type;
}

void LiteralExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

auto LiteralExpr::get_value_string() const -> std::string {
    return std::visit(
        [](auto &&val) -> std::string {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "null";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return val;
            } else if constexpr (std::is_same_v<T, bool>) {
                return val ? "true" : "false";
            } else if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(val);
            } else {
                throw std::runtime_error("Invalid type");
            }
        },
        value);
}

/* ----------------------------[IDENTIFIER]--------------------------------*/

IdentifierExpr::IdentifierExpr(const Position pos, std::string identifier)
    : Expression(pos), identifier(std::move(identifier)) {}

auto IdentifierExpr::get_identifier() const -> std::string { return identifier; }
void IdentifierExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

/* ------------------------------[UNARY]--------------------------------*/

UnaryExpr::UnaryExpr(Position pos, UnaryOp unary_op, std::unique_ptr<Expression> right)
    : Expression(pos), op_type(unary_op), right(std::move(right)) {}

auto UnaryExpr::get_operator() const -> UnaryOp { return op_type; }

void UnaryExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

auto UnaryExpr::get_right() const -> const Expression * { return right.get(); }

/* ------------------------------[BINARY]--------------------------------*/

BinaryExpr::BinaryExpr(Position pos, std::unique_ptr<Expression> left, BinaryOp op,
                       std::unique_ptr<Expression> right)
    : Expression(pos), left(std::move(left)), op(op), right(std::move(right)) {}

void BinaryExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

auto BinaryExpr::get_operator() const -> BinaryOp { return op; }

auto BinaryExpr::get_left() const -> const Expression * { return left.get(); }

auto BinaryExpr::get_right() const -> const Expression * { return right.get(); }

/* ------------------------------[CALL]--------------------------------*/

CallExpr::CallExpr(Position pos, std::unique_ptr<Expression> name,
                   std::vector<std::unique_ptr<Expression>> args)
    : Expression(pos), func_name(std::move(name)), args(std::move(args)) {}

auto CallExpr::get_func_name() const -> const Expression * { return func_name.get(); }

auto CallExpr::get_args() const -> const std::vector<const Expression *> {
    std::vector<const Expression *> result;
    result.reserve(args.size());
    for (const auto &arg : args) {
        result.push_back(arg.get());
    }
    return result;
}

void CallExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

/* ------------------------------[BINDFRT]--------------------------------*/

BindFrtExpr::BindFrtExpr(Position pos, std::unique_ptr<Expression> name,
                         std::vector<std::unique_ptr<Expression>> args)
    : Expression(pos), func_name(std::move(name)), args(std::move(args)) {}

void BindFrtExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

auto BindFrtExpr::get_func_name() const -> const Expression * { return func_name.get(); }

auto BindFrtExpr::get_args() const -> const std::vector<const Expression *> {
    std::vector<const Expression *> result;
    result.reserve(args.size());
    for (const auto &arg : args) {
        result.push_back(arg.get());
    }
    return result;
}
