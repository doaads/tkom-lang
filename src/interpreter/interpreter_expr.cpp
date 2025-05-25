#include "interpreter.h"

/*
 * @brief: function for promoting types to string for the purpose of weak-type arithmetics
 */
template <typename T>
std::string to_string(const T& v) {
    if constexpr (std::is_same_v<T, bool>) {
        return v ? "true" : "false";
    } else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(v);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return v;
    } else {
        throw std::runtime_error("Unsupported type");
    }
}


/*
 * @brief: function for checking if types are of arithmetic rank for 2-argument functions
 */
template <typename L, typename R>
constexpr bool are_of_arithmetic_rank() {
    return std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;
}

/*
 * @brief: function returning the value of an arithmetic function provided as the third argument
 */
template <typename OpFunc>
ValType get_val_for_arithmetic(ValType l, ValType r, OpFunc func) {
    return std::visit(
        [func](auto l, auto r) -> ValType {
            if constexpr (are_of_arithmetic_rank<decltype(l), decltype(r)>()) return func(l, r);
            throw std::runtime_error("Unsupported operator for type");
        },
        l, r);
}

/*
 * @brief: overload template for expression parsing
 *
 * Inspired by: https://www.modernescpp.com/index.php/smart-tricks-with-fold-expressions/
 */
template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

/*
 * @brief: function for handling the '+' operator
 *
 * the function returns the sum of elements for both arithmetic types and strings
 */
ValType InterpreterVisitor::add(ValType lhs, ValType rhs) {
    return std::visit(
        Overload{
            [](const std::string& l, const std::string& r) -> ValType { return l + r; },
            [](const std::string& l, auto r) -> ValType { return l + to_string(r); },
            [](auto l, const std::string& r) -> ValType { return to_string(l) + r; },
            [](auto l, auto r) -> ValType {
                return get_val_for_arithmetic(l, r,
                                              [](auto a, auto b) -> ValType { return a + b; });
            },
        },
        lhs, rhs);
}

/*
 * @brief: function for handling the '-' operator
 *
 * the function returns a value only for arithmetic types
 */
ValType InterpreterVisitor::sub(ValType lhs, ValType rhs) {
    return std::visit(
        [](auto l, auto r) -> ValType {
            return get_val_for_arithmetic(l, r, [](auto a, auto b) -> ValType { return a - b; });
        },
        lhs, rhs);
}

/*
 * @brief: function for handling the '*' operator
 *
 * arithmetic types are multiplied normally, (string, int) combination returns a n-times multiplied string
 */
ValType InterpreterVisitor::mul(ValType lhs, ValType rhs) {
    return std::visit(Overload{[](const std::string& s, int count) -> ValType {
                                   std::string result;
                                   for (int i = 0; i < count; ++i) result += s;
                                   return result;
                               },
                               [](int count, const std::string& s) -> ValType {
                                   std::string result;
                                   for (int i = 0; i < count; ++i) result += s;
                                   return result;
                               },
                               [](auto l, auto r) -> ValType {
                                   return get_val_for_arithmetic(
                                       l, r, [](auto a, auto b) -> ValType { return a * b; });
                               }},
                      lhs, rhs);
}

/*
 * @brief: function for handling the '/' operator
 *
 * works only for arithmetic types
 */
ValType InterpreterVisitor::div(ValType lhs, ValType rhs) {
    return std::visit(
        [](auto l, auto r) -> ValType {
            return get_val_for_arithmetic(l, r, [](auto a, auto b) -> ValType { return a / b; });
        },
        lhs, rhs);
}

/*
 * @brief: function for handling comparison operators (<, >, <=, >=, ==, !=)
 *
 * string comparison only compares string length for both arguments
 */
template <typename CompareFunc>
ValType InterpreterVisitor::compare(ValType lhs, ValType rhs, CompareFunc func) {
    return std::visit(
        Overload{[func](const std::string& l, const std::string& r) -> ValType { return func(l, r); },
                 [func](auto l, auto r) -> ValType { return get_val_for_arithmetic(l, r, func); }},
        lhs, rhs);
}

/*
 * @brief: function for handling unary operators (!, -)
 */
template <typename UnaryFunc>
ValType InterpreterVisitor::unary(ValType val, UnaryFunc func) {
    return std::visit([func](auto l) -> ValType {
                          if constexpr (std::is_arithmetic_v<decltype(l)>) return func(l);
                          throw std::runtime_error("Unsupported operator for type");
                      },
                      val);
}

/*
 * @brief: function for handling unary operators (!, -)
 */
template <typename LogicalFunc>
ValType InterpreterVisitor::logical(ValType lhs, ValType rhs, LogicalFunc func) {
    return std::visit(Overload{
            [func](const std::string& l, const std::string& r) -> ValType { return func(!l.empty(), !r.empty()); },
            [func](auto l, auto r) -> ValType { return get_val_for_arithmetic(l, r, func); }
            }, lhs, rhs);
}

void InterpreterVisitor::visit(const LiteralExpr& expr) {
    ValueType expr_val = expr.get_value();

    current_value = std::visit(
        [](auto&& v) -> ValType { return ValType{std::forward<decltype(v)>(v)}; }, expr_val);
}


void InterpreterVisitor::visit(const BinaryExpr& expr) {
    expr.get_right()->accept(*this);
    ValType right = current_value;

    expr.get_left()->accept(*this);
    ValType left = current_value;

    switch (expr.get_operator()) {
        case BinaryOp::ADD:
            current_value = add(left, right);
            break;
        case BinaryOp::SUB:
            current_value = sub(left, right);
            break;
        case BinaryOp::MULT:
            current_value = mul(left, right);
            break;
        case BinaryOp::DIV:
            current_value = div(left, right);
            break;
        case BinaryOp::LT:
            current_value = compare(left, right, [](auto l, auto r) -> bool { return l < r; });
            break;
        case BinaryOp::GT:
            current_value = compare(left, right, [](auto l, auto r) -> bool { return l > r; });
            break;
        case BinaryOp::LTE:
            current_value = compare(left, right, [](auto l, auto r) -> bool { return l <= r; });
            break;
        case BinaryOp::GTE:
            current_value = compare(left, right, [](auto l, auto r) -> bool { return l >= r; });
            break;
        case BinaryOp::EQ:
            current_value = compare(left, right, [](auto l, auto r) -> bool { return l == r; });
            break;
        case BinaryOp::NEQ:
            current_value = compare(left, right, [](auto l, auto r) -> bool { return l != r; });
            break;
        case BinaryOp::OR:
            current_value = logical(left, right, [](auto l, auto r) -> bool { return l || r; });
            break;
        case BinaryOp::AND:
            current_value = logical(left, right, [](auto l, auto r) -> bool { return l && r; });
            break;
        default:  // TODO: DECORATORS
            throw std::runtime_error("Unimplemented operator");
    }
}

void InterpreterVisitor::visit(const UnaryExpr& expr) {
    expr.get_right()->accept(*this);
    auto left = current_value;

    switch (expr.get_operator()) {
        case UnaryOp::NOT:
            current_value = unary(left, [](auto l) -> ValType { return !l; });
            break;
        case UnaryOp::MINUS:
            current_value = unary(left, [](auto l) -> ValType { return -l; });
            break;
    }
}

