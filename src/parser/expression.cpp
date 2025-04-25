#include "expression.h"

const std::string Expression::expr_type = "Expression";

std::ostream& operator<<(std::ostream& os, const Expression& op) {
    op.print(os);
    return os;
}

LiteralExpr::LiteralExpr(std::unique_ptr<Expression> expr) :
    value(std::move(expr)) {}

const std::string LiteralExpr::expr_type = "LiteralExpr";

LiteralExpr::LiteralExpr(Token token) :
    value(token) {}

void LiteralExpr::print(std::ostream& os, unsigned short indent) const {
    os << "[" << expr_type << ", val: ";
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
    }, value);
    os << "]";
}

const std::string UnaryExpr::expr_type = "UnaryExpr";

UnaryExpr::UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right) :
    op_type(unary_op), right(std::move(right)) {}

UnaryOp UnaryExpr::get_operator() const {
    return op_type;
}

void UnaryExpr::print(std::ostream& os, unsigned short indent) const {
    os << "[" << expr_type << ", op: " << get_operator();
    os << ", val: " << *right << "]" << std::endl;
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

void BinaryExpr::print(std::ostream& os, unsigned short indent) const {
    os << "[" << expr_type << ",\n\tl: " << *left << std::endl;
    os << "\top: " << op << std::endl;
    os << "\tr: " << *right << std::endl;
    os << "]";
}

const std::string CallExpr::expr_type = "CallExpr";

CallExpr::CallExpr(std::string name, std::vector<std::unique_ptr<Expression>> args) : func_name(name), args(std::move(args)) {}

void CallExpr::print(std::ostream& os, unsigned short indent) const {
    os << "[" << expr_type << ", f: " << func_name;
    os << ", arg: (";
    bool first = true;
    for (auto& arg : args) {
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
    os << ")]";
}
