#include "type.h"

VarType::VarType(BaseType type, bool mut) : type(type), mut(mut) {}

FuncType::FuncType(
        std::optional<BaseType> ret_type,
        std::vector<std::unique_ptr<Type>> params) :
    ret_type(ret_type),
    params(std::move(params)) {}
