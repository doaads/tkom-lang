#pragma once

#include <memory>
#include <vector>

#include "node.h"
#include "statement.h"

class ParserPrinter;
class Visitor;

class Block : public Node {
   private:
    std::vector<std::unique_ptr<Statement>> statements;

   public:
    Block(std::vector<std::unique_ptr<Statement>> statements);
    ~Block() = default;

    void accept(Visitor &visitor) const;

    std::vector<const Statement *> get_statements() const;
};
