#include "type.h"

#include "visitor.h"

std::ostream &operator<<(std::ostream &os, const BaseType &type) {
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

std::optional<BaseType> translate_token_to_type(TokenType type) {
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

std::optional<BaseType> get_literal_token_type(TokenType type) {
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

BaseType VarType::get_type() const { return type; }
bool VarType::get_mut() const { return mut; }
void VarType::accept(Visitor &visitor) const { visitor.visit(*this); }

FuncType::FuncType(std::unique_ptr<Type> ret_type, std::vector<std::unique_ptr<Type>> params)
    : ret_type(std::move(ret_type)), params(std::move(params)) {}

const Type *FuncType::get_ret_type() const { return ret_type.get(); }

const std::vector<const Type *> FuncType::get_params() const {
    std::vector<const Type *> result;
    result.reserve(params.size());
    for (const auto &param : params) {
        result.push_back(param.get());
    }
    return result;
}

void FuncType::accept(Visitor &visitor) const { visitor.visit(*this); }

bool VarType::is_equal_to(const Type* other) const {
    if (other == nullptr) return false;
    if (!other->is_func) {
        const VarType* var_other = static_cast<const VarType*>(other);
        return type == var_other->type && mut == var_other->mut;
    }
    return false;
}

bool FuncType::is_equal_to(const Type* other) const {
    if (other == nullptr) return false;
    if (other->is_func) {
        const FuncType* func_other = static_cast<const FuncType*>(other);
        
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
