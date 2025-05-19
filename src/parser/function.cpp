#include "function.h"

#include "block.h"
#include "visitor.h"
#include "statement.h"
#include "type.h"

FuncSignature::FuncSignature(Position pos, std::unique_ptr<Type> ret,
                             std::vector<std::unique_ptr<FuncParam>> args, std::string name)
    : Node(pos), ret_type(std::move(ret)), args(std::move(args)), name(name) {}

void FuncSignature::accept(Visitor &visitor) const { visitor.visit(*this); }

const Type *FuncSignature::get_type() const { return ret_type.get(); }
std::string FuncSignature::get_name() const { return name; }

const std::vector<const FuncParam *> FuncSignature::get_params() const {
    std::vector<const FuncParam *> params;
    for (auto &param : args) {
        params.push_back(param.get());
    }
    return params;
}

Function::Function(std::unique_ptr<FuncSignature> signature, std::unique_ptr<Block> body)
    : signature(std::move(signature)), body(std::move(body)) {
    Function::Node(Function::signature->get_position());
}

void Function::accept(Visitor &visitor) const { visitor.visit(*this); }

const FuncSignature *Function::get_signature() const { return signature.get(); }

const Block *Function::get_body() const { return body.get(); }
