#include "expression.h"

std::string Expression::indent_str(int amount) {
    return std::string(amount * 2, ' ');
}

/* ------------------------------[LITERAL]--------------------------------*/

LiteralExpr::LiteralExpr(std::unique_ptr<Expression> expr) :
    value(std::move(expr)) {}

LiteralExpr::LiteralExpr(Token token) :
    value(token) {}

void LiteralExpr::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

std::variant<const Expression*, Token> LiteralExpr::get_value() const {
    if (value.index()) {
        return std::get<Token>(value);
    } else {
        return std::get<std::unique_ptr<Expression>>(value).get();
    }
}

/* ------------------------------[UNARY]--------------------------------*/

UnaryExpr::UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right) :
    op_type(unary_op), right(std::move(right)) {}

UnaryOp UnaryExpr::get_operator() const {
    return op_type;
}

void UnaryExpr::accept(ParserPrinter& visitor) const {
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

void BinaryExpr::accept(ParserPrinter& visitor) const {
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

void CallExpr::accept(ParserPrinter& visitor) const {
    visitor.visit(*this);
}

/* ------------------------------[BINDFRT]--------------------------------*/

BindFrtExpr::BindFrtExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args) : func_name(std::move(name)), args(std::move(args)) {}

void BindFrtExpr::accept(ParserPrinter& visitor) const {
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
