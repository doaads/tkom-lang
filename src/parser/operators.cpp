#include "operators.h"


std::ostream& operator<<(std::ostream& os, const UnaryOp& op) {
    switch (op) {
        case UnaryOp::NOT: os << "NOT"; break;
        case UnaryOp::MINUS: os << "MINUS"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const BinaryOp& op) {
    switch (op) {
        case BinaryOp::NEQ: os << "NEQ"; break;
        case BinaryOp::OR: os << "OR"; break;
        case BinaryOp::AND: os << "AND"; break;
        case BinaryOp::EQ: os << "EQ"; break;
        case BinaryOp::LT: os << "LT"; break;
        case BinaryOp::LTE: os << "LTE"; break;
        case BinaryOp::GT: os << "GT"; break;
        case BinaryOp::GTE: os << "GTE"; break;
        case BinaryOp::ADD: os << "ADD"; break;
        case BinaryOp::SUB: os << "SUB"; break;
        case BinaryOp::DIV: os << "DIV"; break;
        case BinaryOp::MULT: os << "MULT"; break;
        case BinaryOp::DECORATE: os << "DECORATE"; break;
    }
    return os;
}

std::optional<BinaryOp> token_to_binary_op(TokenType type) {
    switch (type) {
        case TokenType::T_NEQ: return BinaryOp::NEQ;
        case TokenType::T_OR: return BinaryOp::OR;
        case TokenType::T_AND: return BinaryOp::AND;
        case TokenType::T_EQ: return BinaryOp::EQ;
        case TokenType::T_LT: return BinaryOp::LT;
        case TokenType::T_LTE: return BinaryOp::LTE;
        case TokenType::T_GT: return BinaryOp::GT;
        case TokenType::T_GTE: return BinaryOp::GTE;
        case TokenType::T_PLUS: return BinaryOp::ADD;
        case TokenType::T_MINUS: return BinaryOp::SUB;
        case TokenType::T_DIV: return BinaryOp::DIV;
        case TokenType::T_MULT: return BinaryOp::MULT;
        case TokenType::T_DECORATE: return BinaryOp::DECORATE;
        default: return std::nullopt;
    }
}
std::optional<UnaryOp> token_to_unary_op(TokenType type) {
    switch (type) {
        case TokenType::T_MINUS: return UnaryOp::MINUS;
        case TokenType::T_NOT: return UnaryOp::NOT;
        default: return std::nullopt;
    }
}
