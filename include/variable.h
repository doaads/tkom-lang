#pragma once

#include "expression.h"
#include "type.h"

class Visitor;

/**
 * @brief class representing a variable's signature in the syntax tree
 */
class VariableSignature {
    std::unique_ptr<Type> type;  //< the variable's type
    const std::string name;  //< the variable's name
    Position pos;  //< the variable's position in code

   public:
    VariableSignature(std::unique_ptr<Type> type, std::string name);
    VariableSignature(std::unique_ptr<Type> type, std::string name, Position pos);

    void accept(Visitor &visitor) const;

    /**
     * @brief get the type of the variable
     */
    [[nodiscard]] auto get_type() const -> const Type *;

    /**
     * @brief get the name of the variable
     */
    [[nodiscard]] auto get_name() const -> const std::string;
};
