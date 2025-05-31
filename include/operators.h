#pragma once

#include <optional>
#include <ostream>

#include "tokens.h"

/**
 * @brief an enum representing the possible unary operators
 */
enum class UnaryOp { NOT, MINUS };

auto operator<<(std::ostream &os, const UnaryOp &token) -> std::ostream &;

/**
 * @brief an enum representing the possible binary operators
 */
enum class BinaryOp { OR, AND, NEQ, EQ, LT, LTE, GT, GTE, ADD, SUB, DIV, MULT, DECORATE };

auto operator<<(std::ostream &os, const BinaryOp &token) -> std::ostream &;

/**
 * @brief convert a token of a given type to an unary operator
 */
auto token_to_unary_op(TokenType type) -> std::optional<UnaryOp>;

/**
 * @brief convert a token of a given type to a binary operator
 */
auto token_to_binary_op(TokenType type) -> std::optional<BinaryOp>;
