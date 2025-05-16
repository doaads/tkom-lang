#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "node.h"
#include "tokens.h"

class ParserVisitor;

enum class BaseType {
    INT,
    FLT,
    STRING,
    BOOL,
    VOID,
};

std::optional<BaseType> translate_token_to_type(TokenType type);
std::optional<BaseType> get_literal_token_type(TokenType type);
std::ostream &operator<<(std::ostream &os, const BaseType &type);

class Type : public Node {
   public:
    static const bool is_func = false;
    virtual ~Type() = default;
    virtual void accept(ParserVisitor &visitor) const = 0;
};

class VarType : public Type {
   private:
    BaseType type;
    bool mut = false;

   public:
    VarType(BaseType type, bool mut);
    void accept(ParserVisitor &visitor) const override;

    BaseType get_type() const;
    bool get_mut() const;
};

class FuncType : public Type {
   private:
    std::unique_ptr<Type> ret_type;
    std::vector<std::unique_ptr<Type>> params;

   public:
    FuncType(std::unique_ptr<Type> ret_type, std::vector<std::unique_ptr<Type>> params);
    static const bool is_func = true;
    void accept(ParserVisitor &visitor) const override;

    const Type *get_ret_type() const;
    const std::vector<const Type *> get_params() const;
};
