#pragma once

#include "local_function.h"

using ValType =
    std::variant<std::monostate, std::string, int, double, bool, std::shared_ptr<Callable>>;

using TypeType = std::variant<const Type*, BaseType>;

struct VarRef {
    std::shared_ptr<Variable> ref;  //< variable reference
    std::string curr_name;          //< name in current scope
};

struct Variable {
    const VariableSignature& signature;  //< variable type and name as parsed by the parser
    ValType value;
    const Type* get_type() { return signature.get_type(); }
};

struct BlockScope {
    std::vector<std::shared_ptr<Variable>> vars;  //< variables in current scope (block)
};

struct CallStackFrame {
    std::vector<std::shared_ptr<VarRef>> args;  //< variables passed by reference
    std::vector<BlockScope> var_scope;          //< variables originating inside the function
};

enum class ReceivedBy {
    EXPR,
    VAR,
};

using Arg = std::variant<std::shared_ptr<Variable>, ValType>;
using ArgVector = std::vector<Arg>;
