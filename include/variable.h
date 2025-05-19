#pragma once

#include <string>

#include "type.h"

class Visitor;

class FuncParam {
    std::unique_ptr<Type> type;
    std::string name;

   public:
    FuncParam(std::unique_ptr<Type> type, std::string name);

    void accept(Visitor &visitor) const;
    const Type *get_type() const;
};
