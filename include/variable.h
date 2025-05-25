#pragma once

#include "expression.h"
#include "type.h"

class Visitor;

class VariableSignature {
    std::unique_ptr<Type> type;
    std::unique_ptr<Expression> identifier;
    Position pos;

   public:
    VariableSignature(std::unique_ptr<Type> type, std::unique_ptr<Expression> name);
    VariableSignature(std::unique_ptr<Type> type, std::unique_ptr<Expression> name, Position pos);

    void accept(Visitor &visitor) const;
    const Type *get_type() const;
    const Expression *get_name() const;
};
