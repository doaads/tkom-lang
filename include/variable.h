#pragma once

#include "type.h"
#include <string>

class HighOrder {
    public:
        virtual ~HighOrder();
};

class Variable : public HighOrder {
    std::unique_ptr<Type> type;
    std::string name;
    public:
        Variable(std::unique_ptr<Type> type, std::string name);
};

class FuncVar : public HighOrder {
    std::unique_ptr<Type> type;
    std::string name;
    public:
        FuncVar(std::unique_ptr<Type> type, std::string name);
};

