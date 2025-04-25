#include "variable.h"
#include <stdexcept>

Variable::Variable(std::unique_ptr<Type> type, std::string name) : name(name) {
    if (type->is_func) throw std::runtime_error("Expected variable type");
    this->type = std::move(type);
}

FuncVar::FuncVar(std::unique_ptr<Type> type, std::string name) : name(name) {
    if (!type->is_func) throw std::runtime_error("Expected function type");
    this->type = std::move(type);
}
