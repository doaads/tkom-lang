#include "expression.h"

const std::string Expression::expr_type = "Expression";

std::ostream& operator<<(std::ostream& os, const Expression& op) {
    os << op.expr_type << std::endl;
    throw std::runtime_error("Cannot parse abstract expression class");
    return os;
}

LiteralExpr::LiteralExpr(std::unique_ptr<Expression> expr) :
    value(std::move(expr)) {}

const std::string LiteralExpr::expr_type = "LiteralExpr";

LiteralExpr::LiteralExpr(Token token) :
    value(token) {}

std::ostream& operator<<(std::ostream& os, const LiteralExpr& op) {
    os << "[" << op.expr_type << "\n value: ";
    std::visit([&os](const auto& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::unique_ptr<Expression>>) {
            if (val) {
                os << *val;
            } else {
                os << "null";
            }
        } else if constexpr (std::is_same_v<T, Token>) {
            os << val;
        }
    }, op.value);
    return os;
}

const std::string UnaryExpr::expr_type = "UnaryExpr";

UnaryExpr::UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right) :
    op_type(unary_op), right(std::move(right)) {}

UnaryOp UnaryExpr::get_operator() const {
    return op_type;
}

std::ostream& operator<<(std::ostream& os, const UnaryExpr& op) {
    os << "[" << op.expr_type << "\n op: " << op.get_operator() << std::endl;
    os << " value: " << *op.right << std::endl;
    return os;
}

const std::string BinaryExpr::expr_type = "BinaryExpr";

BinaryExpr::BinaryExpr(
        std::unique_ptr<Expression> left,
        BinaryOp op,
        std::unique_ptr<Expression> right) :
    left(std::move(left)), op(op), right(std::move(right)) {}

BinaryOp BinaryExpr::get_operator() const {
    return op;
}

std::ostream& operator<<(std::ostream& os, const BinaryExpr& op) {
    os << "[" << op.expr_type << "\n left: " << *op.left << std::endl;
    os << " op: " << op.op << std::endl;
    os << " value: " << *op.right << std::endl;
    return os;
}

const std::string CallExpr::expr_type = "CallExpr";

CallExpr::CallExpr(std::string name, std::vector<std::unique_ptr<Expression>> args) : func_name(name), args(std::move(args)) {}

std::ostream& operator<<(std::ostream& os, const CallExpr& op) {
    os << "[" << op.expr_type << "\n function: " << op.func_name << std::endl;
    os << " args: (";
    bool first = true;
    for (auto& arg : op.args) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }

        if (arg) {
            os << *arg;
        } else {
            os << "null";
        }
    }
    os << ")";
    return os;
}
