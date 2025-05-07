#include "variable.h"
#include "parser_visitor.h"
#include <stdexcept>

FuncParam::FuncParam(std::unique_ptr<Type> type, std::string name) : name(name) {
    if (type->is_func) throw std::runtime_error("Expected variable type");
    this->type = std::move(type);
}

void FuncParam::accept(ParserVisitor& visitor) const {
    return visitor.visit(*this);
}

const Type* FuncParam::get_type() const {
    return type.get();
}
