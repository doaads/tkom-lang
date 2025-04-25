#pragma once

#include "block.h"
#include "type.h"
#include "variable.h"
#include <memory>

class FuncSignature {
    std::unique_ptr<Type> ret_type;
    std::vector<std::unique_ptr<HighOrder>> args;
    std::string name;
    public:
        FuncSignature(
            std::unique_ptr<Type> ret,
            std::vector<std::unique_ptr<HighOrder>> args,
            std::string name);
};

class Function {
    private:
        std::unique_ptr<FuncSignature> signature;
        std::unique_ptr<Block> body;
    public:
        Function(
                std::unique_ptr<FuncSignature> signature,
                std::unique_ptr<Block> body);
};
