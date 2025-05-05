#pragma once

#include "node.h"
#include <memory>
#include <vector>

class Statement;
class ParserPrinter;
class ParserVisitor;

class Block : public Node {
    private:
        std::vector<std::unique_ptr<Statement>> statements;
    public:
        Block(std::vector<std::unique_ptr<Statement>> statements);
        ~Block() = default;

        void accept(ParserVisitor& visitor) const;

        const std::vector<const Statement*> get_statements() const;
};
