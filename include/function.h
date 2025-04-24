#pragma once

#include "block.h"
#include "variable.h"
#include <memory>


class Function {
    private:
        FuncVar signature;
        std::unique_ptr<Block> body;
    public:
        Function(
                FuncVar signature,
                std::unique_ptr<Block> body);
};
