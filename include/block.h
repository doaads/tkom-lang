#pragma once

#include "statement.h"
#include <memory>
#include <vector>

class Block {
    private:
        std::vector<std::unique_ptr<Statement>> statements;
    public:
        Block(std::vector<std::unique_ptr<Statement>> statements);
};
