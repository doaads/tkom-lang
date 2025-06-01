#include "block.h"

#include "visitor.h"

Block::Block(std::vector<std::unique_ptr<Statement>> statements)
    : statements(std::move(statements)) {}

void Block::accept(Visitor &visitor) const { visitor.visit(*this); }

std::vector<const Statement *> Block::get_statements() const {
    std::vector<const Statement *> result;
    result.reserve(statements.size());
    for (const auto &stmt : statements) {
        result.push_back(stmt.get());
    }
    return result;
}
