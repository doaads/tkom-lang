#include "variable.h"

#include "visitor.h"

VariableSignature::VariableSignature(std::unique_ptr<Type> type, std::unique_ptr<Expression> name) : type(std::move(type)), name(std::move(name)) {}

VariableSignature::VariableSignature(std::unique_ptr<Type> type, std::unique_ptr<Expression> name, Position pos) : type(std::move(type)), name(std::move(name)), pos(pos) {}

void VariableSignature::accept(Visitor &visitor) const { return visitor.visit(*this); }

const Type *VariableSignature::get_type() const { return type.get(); }

const Expression *VariableSignature::get_name() const { return name.get(); }
