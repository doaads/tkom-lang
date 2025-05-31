/**
 * @file builtin_helpers.h
 *
 * Helper functions for easier inplementation of builtin functions
 */
#pragma once

#include <deque>
#include "interpreter_helpers.h"

/**
 * @brief modify a variable's value.
 */
void modify_value(Arg& arg, ValType value);

/**
 * @brief get a variable's value
 */
auto get_value(Arg& arg) -> ValType;

/**
 * @brief build a function type
 *
 * The argument must be a std::deque containing the return type as the first element and argument types following it
 */
auto build_type(std::deque<VarType> args) -> std::unique_ptr<Type>;
