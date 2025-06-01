#include "program.h"

#include "visitor.h"

void Program::accept(Visitor &visitor) const { visitor.visit(*this); }

auto Program::get_functions() const -> const std::vector<const Function *> {
    std::vector<const Function *> result;
    result.reserve(functions.size());
    for (const auto &func : functions) {
        result.push_back(func.get());
    }
    return result;
}
