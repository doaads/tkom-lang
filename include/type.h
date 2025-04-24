#pragma once

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
        virtual ~Type() = default;
        static const bool is_func = false;
};

class VarType : public Type {
    private:
        BaseType type;
        bool mut = false;
    public:
        VarType(BaseType type, bool mut);

};

class FuncType : public Type {
    private:
        std::optional<BaseType> ret_type;
        std::vector<std::unique_ptr<Type>> params;
    public:
        FuncType(std::optional<BaseType> ret_type, std::vector<std::unique_ptr<Type>> params);
        static const bool is_func = true;
};
