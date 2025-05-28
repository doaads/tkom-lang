#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "node.h"
#include "tokens.h"

class Visitor;

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
    virtual void accept(Visitor &visitor) const = 0;

    virtual bool get_mut() const = 0;
    virtual bool is_equal_to(const Type* other) const = 0;

    virtual std::unique_ptr<Type> clone(size_t skip_args = 0) const = 0;
};

class VarType : public Type {
   private:
    BaseType type;
    bool mut = false;

   public:
    VarType(BaseType type, bool mut);
    void accept(Visitor &visitor) const override;

    BaseType get_type() const;
    bool get_mut() const override;
    bool is_equal_to(const Type* other) const override;

    std::unique_ptr<Type> clone(size_t = 0) const override;
};

class FuncType : public Type {
   private:
    std::unique_ptr<Type> ret_type;
    std::vector<std::unique_ptr<Type>> params;

   public:
    FuncType(std::unique_ptr<Type> ret_type, std::vector<std::unique_ptr<Type>> params);
    static const bool is_func = true;
    void accept(Visitor &visitor) const override;

    bool get_mut() const override { return false; }
    const Type *get_ret_type() const;
    const std::vector<const Type *> get_params() const;
    bool is_equal_to(const Type* other) const override;

    std::unique_ptr<Type> clone(size_t skip_args = 0) const override;
};
