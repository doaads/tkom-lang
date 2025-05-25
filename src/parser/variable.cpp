#include "variable.h"

#include "visitor.h"

VariableSignature::VariableSignature(std::unique_ptr<Type> type, std::string name) : type(std::move(type)), name(name) {}

VariableSignature::VariableSignature(std::unique_ptr<Type> type, std::string name, Position pos) : type(std::move(type)), name(name), pos(pos) {}

void VariableSignature::accept(Visitor &visitor) const { return visitor.visit(*this); }

const Type *VariableSignature::get_type() const { return type.get(); }

const std::string VariableSignature::get_name() const { return name; }
