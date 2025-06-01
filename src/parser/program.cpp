#include "program.h"

#include "visitor.h"

void Program::accept(Visitor &visitor) const { visitor.visit(*this); }

const std::vector<const Function *> Program::get_functions() const {
    std::vector<const Function *> result;
    result.reserve(functions.size());
    for (const auto &func : functions) {
        result.push_back(func.get());
    }
    return result;
}
