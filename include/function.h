#pragma once

#include <memory>

#include "block.h"
#include "type.h"
#include "variable.h"

class Visitor;

/**
 * @brief class representing a function's signature
 * 
 * the class stores a unique pointer to the return type of the function, and the signatures of all arguments it accepts
 * as well as its name
 */
class FuncSignature : public Node {
    std::unique_ptr<Type> ret_type;  //< the return type of the function
    std::vector<std::unique_ptr<VariableSignature>> args;  //< the signature of the function's arguments
    std::string name;  //< the name of the function

   public:
    FuncSignature(Position pos, std::unique_ptr<Type> ret,
                  std::vector<std::unique_ptr<VariableSignature>> args, std::string name);

    void accept(Visitor &visitor) const;

    /**
     * @brief get the function's return type
     */
    [[nodiscard]] auto get_type() const -> const Type * { return ret_type.get(); };

    /**
     * @brief clone the function's signature as a `FuncType`
     */
    [[nodiscard]] auto clone_type_as_type_obj() const -> std::unique_ptr<FuncType>;

    /**
     * @brief get the function's parameter
     */
    [[nodiscard]] auto get_params() const -> const std::vector<const VariableSignature *>;

    /**
     * @brief get the function's name
     */
    [[nodiscard]] auto get_name() const -> std::string;
};

/**
 * @brief class representing a parsed function
 */
class Function : public Node {
   private:
    std::unique_ptr<FuncSignature> signature;  //< the signature of the function
    std::unique_ptr<Block> body;  //< the function's body

   public:
    Function(std::unique_ptr<FuncSignature> signature, std::unique_ptr<Block> body);
    void accept(Visitor &visitor) const;

    /**
     * @brief get the function's signature
     */
    [[nodiscard]] auto get_signature() const -> const FuncSignature *;

    /**
     * @brief get the function's body
     */
    [[nodiscard]] auto get_body() const -> const Block *;
};
