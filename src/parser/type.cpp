#include "type.h"

#include "visitor.h"

auto operator<<(std::ostream &os, const BaseType &type) -> std::ostream & {
    switch (type) {
        case BaseType::INT:
            os << "int";
            break;
        case BaseType::FLT:
            os << "flt";
            break;
        case BaseType::STRING:
            os << "string";
            break;
        case BaseType::BOOL:
            os << "bool";
            break;
        case BaseType::VOID:
            os << "void";
            break;
    }
    return os;
}

auto translate_token_to_type(TokenType type) -> std::optional<BaseType> {
    switch (type) {
        case TokenType::T_INT_TYPE:
            return BaseType::INT;
        case TokenType::T_FLT_TYPE:
            return BaseType::FLT;
        case TokenType::T_STRING_TYPE:
            return BaseType::STRING;
        case TokenType::T_BOOL_TYPE:
            return BaseType::BOOL;
        case TokenType::T_VOID_TYPE:
            return BaseType::VOID;
        default:
            return std::nullopt;
    }
}

auto get_literal_token_type(TokenType type) -> std::optional<BaseType> {
    switch (type) {
        case TokenType::T_INT:
            return BaseType::INT;
        case TokenType::T_FLT:
            return BaseType::FLT;
        case TokenType::T_STRING:
            return BaseType::STRING;
        case TokenType::T_BOOL:
            return BaseType::BOOL;
        case TokenType::T_IDENTIFIER:
            return BaseType::STRING;
        default:
            return std::nullopt;
    }
}

VarType::VarType(BaseType type, bool mut) : type(type), mut(mut) {}

auto VarType::get_type() const -> BaseType { return type; }
auto VarType::get_mut() const -> bool { return mut; }
void VarType::accept(Visitor &visitor) const { visitor.visit(*this); }

FuncType::FuncType(std::unique_ptr<Type> ret_type, std::vector<std::unique_ptr<Type>> params)
    : ret_type(std::move(ret_type)), params(std::move(params)) {}

auto FuncType::get_ret_type() const -> const Type * { return ret_type.get(); }

auto FuncType::get_params() const -> std::vector<const Type *> {
    std::vector<const Type *> result;
    result.reserve(params.size());
    for (const auto &param : params) {
        result.push_back(param.get());
    }
    return result;
}

void FuncType::accept(Visitor &visitor) const { visitor.visit(*this); }

auto VarType::is_equal_to(const Type *other) const -> bool {
    if (other == nullptr) return false;
    if (!other->is_func()) {
        const auto *var_other = static_cast<const VarType *>(other);
        return type == var_other->type && mut == var_other->mut;
    }
    return false;
}

auto FuncType::is_equal_to(const Type *other) const -> bool {
    if (other == nullptr) return false;
    if (other->is_func()) {
        const auto *func_other = static_cast<const FuncType *>(other);

        if (!ret_type->is_equal_to(func_other->ret_type.get())) {
            return false;
        }
        
        if (params.size() != func_other->params.size()) {
            return false;
        }
        
        for (size_t i = 0; i < params.size(); ++i) {
            if (!params[i]->is_equal_to(func_other->params[i].get())) {
                return false;
            }
        }
        
        return true;
    }
    return false;
}

auto VarType::clone(size_t) const -> std::unique_ptr<Type> {
    return std::make_unique<VarType>(type, mut);
}

auto FuncType::clone(size_t skip_args) const -> std::unique_ptr<Type> {
    if (skip_args > params.size()) {
        throw std::out_of_range("FuncType::clone: skip_args out of range");
    }

    std::vector<std::unique_ptr<Type>> new_params;
    for (size_t i = skip_args; i < params.size(); ++i) {
        if (params[i])
            new_params.push_back(params[i]->clone());
    }

    auto result = std::make_unique<FuncType>(ret_type->clone(), std::move(new_params));
    return result;
}
