#pragma once

#include "position.h"

/**
 * @brief Class representing a single node in the AST
 * The class stores data on a block's position after being parsed
 * This is the class from which all AST elements derive
 */
class Node {
   private:
    const Position pos;  //< block's position

   public:
    Node() = default;

    /**
     * Constructor accepting block's position
     */
    Node(Position pos) : pos(pos) {}
    virtual ~Node() = default;

    /**
     * get the block's position
     */
    const Position get_position() const { return pos; }
};
