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
auto translate_token_to_type(TokenType type) -> std::optional<BaseType>;

/**
 * @brief function for translating non-void types to BaseType
 */
auto get_literal_token_type(TokenType type) -> std::optional<BaseType>;
auto operator<<(std::ostream &os, const BaseType &type) -> std::ostream &;

/**
 * @brief Abstract class representing a Type
 */
class Type : public Node {
   public:
    /**
     * @brief function returning true if the type is a function type
     */
    [[nodiscard]] virtual auto is_func() const -> bool { return false; };
    ~Type() override = default;
    virtual void accept(Visitor &visitor) const = 0;

    /**
     * @brief function returning true if the type represented is mutable
     */
    [[nodiscard]] virtual auto get_mut() const -> bool = 0;

    /**
     * @brief function for checking type-equality during semantic analysis
     *
     * @param other Pointer to another type object
     */
    virtual auto is_equal_to(const Type *other) const -> bool = 0;

    /**
     * @brief function returning the parameters of a function type
     */
    [[nodiscard]] virtual auto get_params() const -> std::vector<const Type *> = 0;

    /**
     * @brief function returning the return type of a function, or VarType for variables
     */
    [[nodiscard]] virtual auto get_ret_type() const -> const Type * = 0;

    /**
     * @brief function for type copying
     *
     * @param skip_args how many args to skip (for bind front)
     */
    [[nodiscard]] virtual auto clone(size_t skip_args = 0) const -> std::unique_ptr<Type> = 0;
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
    [[nodiscard]] auto get_type() const -> BaseType;

    /**
     * @brief returns true if the type is mutable
     */
    [[nodiscard]] auto get_mut() const -> bool override;

    /**
     * @brief function for checking type equality
     *
     * @param other the pointer to another Type object
     */
    auto is_equal_to(const Type *other) const -> bool override;

    /**
     * @brief function returning the parameters of a function type
     */
    [[nodiscard]] auto get_params() const -> std::vector<const Type *> override {
        throw std::runtime_error("Variable type does not contain parameters");
    }
    [[nodiscard]] auto get_ret_type() const -> const Type * override { return this; }

    /**
     * @brief function for cloning this type
     */
    [[nodiscard]] auto clone(size_t = 0) const -> std::unique_ptr<Type> override;
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
    [[nodiscard]] auto is_func() const -> bool override { return true; }
    void accept(Visitor &visitor) const override;

    /**
     * @brief function for checking if a FuncType is mutable, will always return false
     */
    [[nodiscard]] auto get_mut() const -> bool override { return false; }

    /**
     * @brief get the return type of a function
     */
    [[nodiscard]] auto get_ret_type() const -> const Type * override;

    /**
     * @brief get the parameters of a function
     */
    [[nodiscard]] auto get_params() const -> std::vector<const Type *> override;

    /**
     * @brief check if a function type is equal to another
     */
    auto is_equal_to(const Type *other) const -> bool override;

    /**
     * @brief clone a function type
     * @param skip_args how many args to skip
     */
    [[nodiscard]] auto clone(size_t skip_args = 0) const -> std::unique_ptr<Type> override;
};
