#include "expression.h"

#include <string>

#include "visitor.h"
#include "type.h"

/* ------------------------------[LITERAL]--------------------------------*/

LiteralExpr::LiteralExpr(const Position pos, Token token)
    : Expression(pos), value(token.get_value()) {
    std::optional<BaseType> type = get_literal_token_type(token.get_type());
    if (!type) throw std::runtime_error("Invalid type passed on to LiteralExpr");
    LiteralExpr::type = *type;
}

void LiteralExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

struct LiteralString {
    const std::string operator()(std::monostate) { return "null"; }
    const std::string operator()(const std::string& rhs) { return rhs; }
    const std::string operator()(bool rhs) { return rhs ? "true" : "false"; }
    const std::string operator()(auto rhs) { return std::to_string(rhs); }
};

std::string LiteralExpr::get_value_string() const {
    return std::visit(LiteralString(), value);
}

/* ----------------------------[IDENTIFIER]--------------------------------*/

IdentifierExpr::IdentifierExpr(const Position pos, std::string identifier)
    : Expression(pos), identifier(identifier) {}

std::string IdentifierExpr::get_identifier() const { return identifier; }
void IdentifierExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

/* ------------------------------[UNARY]--------------------------------*/

UnaryExpr::UnaryExpr(Position pos, UnaryOp unary_op, std::unique_ptr<Expression> right)
    : Expression(pos), op_type(unary_op), right(std::move(right)) {}

UnaryOp UnaryExpr::get_operator() const { return op_type; }

void UnaryExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

const Expression *UnaryExpr::get_right() const { return right.get(); }

/* ------------------------------[BINARY]--------------------------------*/

BinaryExpr::BinaryExpr(Position pos, std::unique_ptr<Expression> left, BinaryOp op,
                       std::unique_ptr<Expression> right)
    : Expression(pos), left(std::move(left)), op(op), right(std::move(right)) {}

void BinaryExpr::accept(Visitor &visitor) const { visitor.visit(*this); }

BinaryOp BinaryExpr::get_operator() const { return op; }

const Expression *BinaryExpr::get_left() const { return left.get(); }

const Expression *BinaryExpr::get_right() const { return right.get(); }

/* ------------------------------[CALL]--------------------------------*/

CallExpr::CallExpr(Position pos, std::unique_ptr<Expression> name,
                   std::vector<std::unique_ptr<Expression>> args)
    : Expression(pos), func_name(std::move(name)), args(std::move(args)) {}

const Expression *CallExpr::get_func_name() const { return func_name.get(); }

const std::vector<const Expression *> CallExpr::get_args() const {
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

const Expression *BindFrtExpr::get_func_name() const { return func_name.get(); }

const std::vector<const Expression *> BindFrtExpr::get_args() const {
    std::vector<const Expression *> result;
    result.reserve(args.size());
    for (const auto &arg : args) {
        result.push_back(arg.get());
    }
    return result;
}
