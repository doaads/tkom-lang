#pragma once

#include "statement.h"
#include <memory>
#include <vector>

class Statement;

class Block {
    private:
        std::vector<std::unique_ptr<Statement>> statements;
    public:
        Block(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(statements)) {}
        ~Block() = default;

        void accept(ParserPrinter& visitor) const;
};
