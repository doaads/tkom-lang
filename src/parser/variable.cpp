#include "variable.h"
#include "parser_visitor.h"
#include <stdexcept>

Variable::Variable(std::unique_ptr<Type> type, std::string name) : name(name) {
    if (type->is_func) throw std::runtime_error("Expected variable type");
    this->type = std::move(type);
}

void Variable::accept(ParserVisitor& visitor) const {
    return visitor.visit(*this);
}

const Type* Variable::get_type() const {
    return type.get();
}
