#pragma once

#include <memory>

#include "block.h"
#include "type.h"
#include "variable.h"

class Visitor;


class FuncSignature : public Node {
    std::unique_ptr<Type> ret_type;
    std::vector<std::unique_ptr<VariableSignature>> args;
    std::string name;

   public:
    FuncSignature(Position pos, std::unique_ptr<Type> ret,
                  std::vector<std::unique_ptr<VariableSignature>> args, std::string name);
    void accept(Visitor &visitor) const;
    const Type *get_type() const;
    const std::vector<const VariableSignature *> get_params() const;
    std::string get_name() const;
};

class Function : public Node {
   private:
    std::unique_ptr<FuncSignature> signature;
    std::unique_ptr<Block> body;

   public:
    Function(std::unique_ptr<FuncSignature> signature, std::unique_ptr<Block> body);
    void accept(Visitor &visitor) const;
    const FuncSignature *get_signature() const;
    const Block *get_body() const;
};
