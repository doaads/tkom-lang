#include "block.h"

#include "parser_visitor.h"

Block::Block(std::vector<std::unique_ptr<Statement>> statements)
    : statements(std::move(statements)) {}

void Block::accept(ParserVisitor &visitor) const { visitor.visit(*this); }

std::vector<const Statement *> Block::get_statements() const {
    std::vector<const Statement *> result;
    result.reserve(statements.size());
    for (const auto &stmt : statements) {
        result.push_back(stmt.get());
    }
    return result;
}
