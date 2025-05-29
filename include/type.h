#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "node.h"
#include "tokens.h"

class Visitor;

/**
 * @brief enum representing a base type (no function types)
 */
enum class BaseType {
    INT,
    FLT,
    STRING,
    BOOL,
    VOID,
};

/**
 * @brief function for translating a TokenType to BaseType
 */
std::optional<BaseType> translate_token_to_type(TokenType type);

/**
 * @brief function for translating non-void types to BaseType
 */
std::optional<BaseType> get_literal_token_type(TokenType type);
std::ostream &operator<<(std::ostream &os, const BaseType &type);

/**
 * @brief Abstract class representing a Type
 */
class Type : public Node {
   public:
    /**
     * @brief function returning true if the type is a function type
     */
    virtual bool is_func() const {
        return false;
    };
    virtual ~Type() = default;
    virtual void accept(Visitor &visitor) const = 0;

    /**
     * @brief function returning true if the type represented is mutable
     */
    virtual bool get_mut() const = 0;

    /**
     * @brief function for checking type-equality during semantic analysis
     *
     * @param other Pointer to another type object
     */
    virtual bool is_equal_to(const Type* other) const = 0;

    /**
     * @brief function returning the parameters of a function type
     */
    virtual std::vector<const Type*> get_params() const = 0;

    /**
     * @brief function returning the return type of a function, or VarType for variables
     */
    virtual const Type* get_ret_type() const = 0;

    /**
     * @brief function for type copying
     *
     * @param skip_args how many args to skip (for bind front)
     */
    virtual std::unique_ptr<Type> clone(size_t skip_args = 0) const = 0;
};

/*
 * @brief class representing a variable type
 */
class VarType : public Type {
   private:
    BaseType type;  //< the base type of the variable
    bool mut = false;  //< whether the variable is mutable

   public:

    /**
     * @brief The main constructor for the VarType class
     *
     * @param type the type of the variable
     * @param mut whether the variable is mutable
     */
    VarType(BaseType type, bool mut);

    void accept(Visitor &visitor) const override;
    
    /**
     * @brief get the current type
     */
    BaseType get_type() const;

    /**
     * @brief returns true if the type is mutable
     */
    bool get_mut() const override;

    /**
     * @brief function for checking type equality
     *
     * @param other the pointer to another Type object
     */
    bool is_equal_to(const Type* other) const override;

    /**
     * @brief function returning the parameters of a function type
     */
    std::vector<const Type*> get_params() const override { throw std::runtime_error("Variable type does not contain parameters");}
    const Type* get_ret_type() const override { return this; }

    /**
     * @brief function for cloning this type
     */
    std::unique_ptr<Type> clone(size_t = 0) const override;
};

/**
 * @brief class representing a function type
 */
class FuncType : public Type {
   private:
    std::unique_ptr<Type> ret_type;  //< the return type of the function
    std::vector<std::unique_ptr<Type>> params;  //< the parameters of the function

   public:
    /**
     * @brief the constructor for FuncType
     * @param ret_type the return type of the function
     * @param params vector of the function's parameters
     */
    FuncType(std::unique_ptr<Type> ret_type, std::vector<std::unique_ptr<Type>> params);
    bool is_func() const override { return true; }
    void accept(Visitor &visitor) const override;

    /**
     * @brief function for checking if a FuncType is mutable, will always return false
     */
    bool get_mut() const override { return false; }

    /**
     * @brief get the return type of a function
     */
    const Type *get_ret_type() const override;

    /**
     * @brief get the parameters of a function
     */
    std::vector<const Type *> get_params() const override;

    /**
     * @brief check if a function type is equal to another
     */
    bool is_equal_to(const Type* other) const override;

    /**
     * @brief clone a function type
     * @param skip_args how many args to skip
     */
    std::unique_ptr<Type> clone(size_t skip_args = 0) const override;
};
