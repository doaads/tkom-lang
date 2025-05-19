#pragma once

#include <memory>
#include <vector>

#include "function.h"

class Visitor;

class Program {
   private:
    std::vector<std::unique_ptr<Function>> functions;

   public:
    Program(std::vector<std::unique_ptr<Function>> functions) : functions(std::move(functions)) {}

    void accept(Visitor &visitor) const;

    const std::vector<const Function *> get_functions() const;
};
