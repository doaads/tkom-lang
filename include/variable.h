#pragma once

#include "expression.h"
#include "type.h"

class Visitor;

class VariableSignature {
    std::unique_ptr<Type> type;
    const std::string name;
    Position pos;

   public:
    VariableSignature(std::unique_ptr<Type> type, std::string name);
    VariableSignature(std::unique_ptr<Type> type, std::string name, Position pos);

    void accept(Visitor &visitor) const;
    const Type *get_type() const;
    const std::string get_name() const;
};
