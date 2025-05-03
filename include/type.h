#pragma once

#include "parser_visitor.h"
#include <memory>
#include <optional>
#include <vector>


enum class BaseType {
    INT,
    FLT,
    STRING,
    BOOL,
};

class Type {
    public:
        static const bool is_func = false;
        virtual ~Type() = default;
        virtual void accept(ParserPrinter& visitor) const = 0;
};

class VarType : public Type {
    private:
        BaseType type;
        bool mut = false;
    public:
        VarType(BaseType type, bool mut);
        void accept(ParserPrinter& visitor) const override;

};

class FuncType : public Type {
    private:
        std::optional<BaseType> ret_type;
        std::vector<std::unique_ptr<Type>> params;
    public:
        FuncType(std::optional<BaseType> ret_type, std::vector<std::unique_ptr<Type>> params);
        static const bool is_func = true;
        void accept(ParserPrinter& visitor) const override;
};
