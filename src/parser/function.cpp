#include "function.h"
#include "statement.h"
#include "block.h"
#include "type.h"

FuncSignature::FuncSignature(
        std::unique_ptr<Type> ret,
        std::vector<std::unique_ptr<HighOrder>> args,
        std::string name) :
    ret_type(std::move(ret)),
    args(std::move(args)),
    name(name) {}

Function::Function(
        std::unique_ptr<FuncSignature> signature,
        std::unique_ptr<Block> body) :
    signature(std::move(signature)),
    body(std::move(body)) {}

