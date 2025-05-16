#pragma once

#include <string>

#include "type.h"

class ParserVisitor;

class FuncParam {
    std::unique_ptr<Type> type;
    std::string name;

   public:
    FuncParam(std::unique_ptr<Type> type, std::string name);

    void accept(ParserVisitor &visitor) const;
    const Type *get_type() const;
};
