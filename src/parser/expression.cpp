#include "expression.h"

const std::string Expression::expr_type = "Expression";

std::string Expression::indent_str(int amount) {
    return std::string(amount * 2, ' ');
}

std::ostream& operator<<(std::ostream& os, const Expression& op) {
    op.print(os);
    return os;
}

LiteralExpr::LiteralExpr(std::unique_ptr<Expression> expr) :
    value(std::move(expr)) {}

const std::string LiteralExpr::expr_type = "LiteralExpr";

LiteralExpr::LiteralExpr(Token token) :
    value(token) {}

void LiteralExpr::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "LiteralExpr: ";
    if (std::holds_alternative<Token>(value)) {
        os << std::get<Token>(value);
    } else {
        os << "\n";
        std::get<std::unique_ptr<Expression>>(value)->print(os, indent + 1);
    }
    os << "\n";
}

const std::string UnaryExpr::expr_type = "UnaryExpr";

UnaryExpr::UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right) :
    op_type(unary_op), right(std::move(right)) {}

UnaryOp UnaryExpr::get_operator() const {
    return op_type;
}

void UnaryExpr::print(std::ostream& os, int indent) const {
    os << indent_str(indent);
    os << "[" << expr_type << ", op: " << get_operator();
    os << ", val: ";
    right->print(os, indent + 1);
    os << "]" << std::endl;
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

void BinaryExpr::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "BinaryExpr " << op << "\n";
    left->print(os, indent + 1);
    //os << indent_str(indent + 1) << "Operator: " << op << "\n";
    right->print(os, indent + 1);
}

const std::string CallExpr::expr_type = "CallExpr";

CallExpr::CallExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args) : func_name(std::move(name)), args(std::move(args)) {}

void CallExpr::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "CallExpr\n";
    os << indent_str(indent + 1) << "Function: ";
    func_name->print(os, indent + 1);
    os << indent_str(indent + 1) << "Arguments:\n";

    if (args.empty()) {
        os << indent_str(indent + 2) << "(none)\n";
    } else {
        for (const auto& arg : args) {
            if (arg) {
                arg->print(os, indent + 2);
            } else {
                os << indent_str(indent + 2) << "null\n";
            }
        }
    }
}

BindFrtExpr::BindFrtExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args) : func_name(std::move(name)), args(std::move(args)) {}

void BindFrtExpr::print(std::ostream& os, int indent) const {
    os << indent_str(indent) << "BindFrtExpr\n";
    os << indent_str(indent + 1) << "Function: ";
    func_name->print(os, indent + 1);
    os << indent_str(indent + 1) << "Arguments:\n";

    if (args.empty()) {
        os << indent_str(indent + 2) << "(none)\n";
    } else {
        for (const auto& arg : args) {
            if (arg) {
                arg->print(os, indent + 2);
            } else {
                os << indent_str(indent + 2) << "null\n";
            }
        }
    }
}
