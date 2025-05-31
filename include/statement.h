#pragma once
/**
 * @file statement.h
 *
 * @brief Contains the definition of the Statement class and related structures.
 *
 * This file defines the `Statement` class, which serves as the base class for various types of
 * statements in the abstract syntax tree (AST).
 * and type aliases used for working with statements. Each type of statement (e.g., assignment,
 * conditionals, loops) is represented by a subclass of `Statement` and implements the `accept`
 * method for interacting with a `Visitor`.
 */

#include <memory>
#include <string>

#include "expression.h"
#include "node.h"
#include "type.h"

class Expression;
class Block;
class Visitor;
class Type;
struct ForLoopArgs;
enum class TokenType;

using ExprPtr = std::unique_ptr<Expression>;
using BlockPtr = std::unique_ptr<Block>;

/**
 * @brief Abstract base class for all statement types in the program tree.
 *
 * The `Statement` class represents a statement in program tree. It is the
 * base class for all specific statement types such as conditionals, loops, assignments, and
 * return statements. The `Statement` class provides an abstract `accept` method, which must
 * be implemented by each derived class to interact with a `Visitor`.
 */
class Statement : public Node {
   protected:
    static auto indent_str(int amount) -> std::string;

   public:
    Statement(const Position pos) : Node(pos) {}

    virtual void accept(Visitor &visitor) const = 0;
};
