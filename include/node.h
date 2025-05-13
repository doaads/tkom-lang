#pragma once

#include "position.h"

class Node {
   private:
    const Position pos;

   public:
    Node() = default;
    Node(Position pos) : pos(pos) {}
    virtual ~Node() = default;
    const Position get_position() const { return pos; }
};
