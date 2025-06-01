#pragma once

#include <memory>
#include <vector>

#include "node.h"
#include "statement.h"

class ParserPrinter;
class Visitor;

/**
 * @brief class representing a block of code consisting of statements
 */
class Block : public Node {
   private:
    std::vector<std::unique_ptr<Statement>> statements;  //< the block's body

   public:
    Block(std::vector<std::unique_ptr<Statement>> statements);
    ~Block() override = default;

    void accept(Visitor &visitor) const;

    /**
     * @brief get the block's body as a vector of const Statement*
     */
    [[nodiscard]] auto get_statements() const -> std::vector<const Statement *>;
};
