#pragma once

#include "type.h"
#include <string>

class ParserVisitor;

class Variable {
    std::unique_ptr<Type> type;
    std::string name;
    public:
        Variable(std::unique_ptr<Type> type, std::string name);

        void accept(ParserVisitor& visitor) const;
        const Type* get_type() const;
};

