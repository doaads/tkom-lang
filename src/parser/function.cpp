#include "function.h"

#include <utility>

#include "block.h"
#include "statement.h"
#include "type.h"
#include "visitor.h"

FuncSignature::FuncSignature(Position pos, std::unique_ptr<Type> ret,
                             std::vector<std::unique_ptr<VariableSignature>> args, std::string name)
    : Node(pos), ret_type(std::move(ret)), args(std::move(args)), name(std::move(name)) {}

void FuncSignature::accept(Visitor &visitor) const { visitor.visit(*this); }

auto FuncSignature::clone_type_as_type_obj() const -> std::unique_ptr<FuncType> {
    std::vector<std::unique_ptr<Type>> params;
    for (auto& arg : args) {
        params.push_back(arg->get_type()->clone());
    }
    return std::make_unique<FuncType>(ret_type->clone(), std::move(params));
}

auto FuncSignature::get_name() const -> std::string { return name; }

auto FuncSignature::get_params() const -> const std::vector<const VariableSignature *> {
    std::vector<const VariableSignature *> params;
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

auto Function::get_signature() const -> const FuncSignature * { return signature.get(); }

auto Function::get_body() const -> const Block * { return body.get(); }
