#pragma once

#include "function.h"
#include <memory>
#include <vector>

class ParserVisitor;

class Program {
    private:
        std::vector<std::unique_ptr<Function>> functions;
    public:
        Program(std::vector<std::unique_ptr<Function>> functions) :
            functions(std::move(functions)) {}

        void accept(ParserVisitor& visitor) const;

        const std::vector<const Function*> get_functions() const;
};
