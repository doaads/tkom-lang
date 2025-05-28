#include "arithmetics.h"
#include "interpreter.h"

static Add add_v{};
static Sub sub_v{};
static Mul mul_v{};
static Div div_v{};

static Compare lt_v{[](auto l, auto r) -> bool { return l < r; }};
static Compare gt_v{[](auto l, auto r) -> bool { return l > r; }};
static Compare lte_v{[](auto l, auto r) -> bool { return l <= r; }};
static Compare gte_v{[](auto l, auto r) -> bool { return l >= r; }};
static Compare eq_v{[](auto l, auto r) -> bool { return l == r; }};
static Compare neq_v{[](auto l, auto r) -> bool { return l != r; }};

static Logical and_v{[](auto l, auto r) -> bool { return l && r; }};
static Logical or_v{[](auto l, auto r) -> bool { return l || r; }};

static Unary unary_not_v{[](auto l) -> bool { return !l; }};
static Unary unary_minus_v{[](auto l) -> int { return -l; }};

static Decorate decorator_v{};

void InterpreterVisitor::visit(const LiteralExpr& expr) {
    ValueType expr_val = expr.get_value();

    current_value = std::visit(
        [](auto&& v) -> ValType { return ValType{std::forward<decltype(v)>(v)}; }, expr_val);
    receiver = ReceivedBy::EXPR;
}

void InterpreterVisitor::visit(const BinaryExpr& expr) {
    expr.get_right()->accept(*this);
    ValType right = current_value;

    expr.get_left()->accept(*this);
    ValType left = current_value;

    switch (expr.get_operator()) {
        case BinaryOp::ADD:
            current_value = std::visit(add_v, left, right);
            break;
        case BinaryOp::SUB:
            current_value = std::visit(sub_v, left, right);
            break;
        case BinaryOp::MULT:
            current_value = std::visit(mul_v, left, right);
            break;
        case BinaryOp::DIV:
            current_value = std::visit(div_v, left, right);
            break;
        case BinaryOp::LT:
            current_value = std::visit(lt_v, left, right);
            break;
        case BinaryOp::GT:
            current_value = std::visit(gt_v, left, right);
            break;
        case BinaryOp::LTE:
            current_value = std::visit(lte_v, left, right);
            break;
        case BinaryOp::GTE:
            current_value = std::visit(gte_v, left, right);
            break;
        case BinaryOp::EQ:
            current_value = std::visit(eq_v, left, right);
            break;
        case BinaryOp::NEQ:
            current_value = std::visit(neq_v, left, right);
            break;
        case BinaryOp::OR:
            current_value = std::visit(or_v, left, right);
            break;
        case BinaryOp::AND:
            current_value = std::visit(and_v, left, right);
            break;
        case BinaryOp::DECORATE:
            current_value = std::visit(decorator_v, left, right);
            break;
    }
    receiver = ReceivedBy::EXPR;
}

void InterpreterVisitor::visit(const UnaryExpr& expr) {
    expr.get_right()->accept(*this);
    auto left = current_value;

    switch (expr.get_operator()) {
        case UnaryOp::NOT:
            current_value = std::visit(unary_not_v, left);
            break;
        case UnaryOp::MINUS:
            current_value = std::visit(unary_minus_v, left);
            break;
    }
}
