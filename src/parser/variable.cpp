#include "variable.h"

#include <utility>

#include "visitor.h"

VariableSignature::VariableSignature(std::unique_ptr<Type> type, std::string name)
    : type(std::move(type)), name(std::move(name)) {}

VariableSignature::VariableSignature(std::unique_ptr<Type> type, std::string name, Position pos)
    : type(std::move(type)), name(std::move(name)), pos(pos) {}

void VariableSignature::accept(Visitor &visitor) const { return visitor.visit(*this); }

auto VariableSignature::get_type() const -> const Type * { return type.get(); }

auto VariableSignature::get_name() const -> const std::string { return name; }
