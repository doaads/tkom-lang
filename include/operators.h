#pragma once

#include "tokens.h"
#include <optional>
#include <ostream>
enum class UnaryOp {
    NOT,
    MINUS
};

std::ostream& operator<<(std::ostream& os, const UnaryOp& token);


enum class BinaryOp {
    OR,
    AND,
    NEQ,
    EQ,
    LT,
    LTE,
    GT,
    GTE,
    ADD,
    SUB,
    DIV,
    MULT
};

std::ostream& operator<<(std::ostream& os, const BinaryOp& token);

std::optional<UnaryOp> token_to_unary_op(TokenType type);
std::optional<BinaryOp> token_to_binary_op(TokenType type);

