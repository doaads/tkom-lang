#pragma once
/**
 * @file interpreter_helpers.h
 *
 * Header containing definitions of interpreter helper structures and enums
 */

#include "local_function.h"

/**
 * @brief value returned by functions/expressions/variables
 */
using ValType =
    std::variant<std::monostate, std::string, int, double, bool, std::shared_ptr<Callable>>;

/**
 * @brief type to BaseType conversion
 */
using TypeType = std::variant<const Type*, BaseType>;

/**
 * @brief variable reference
 *
 * The struct holds a pointer to the original variable and a new name under which it appears
 */
struct VarRef {
    std::shared_ptr<Variable> ref;  //< variable reference
    std::string curr_name;          //< name in current scope
};


/**
 * @brief the variable definition
 *
 * A variable contains its own signature and value
 */
struct Variable {
    const VariableSignature& signature;  //< variable type and name as parsed by the parser
    ValType value;
    auto get_type() -> const Type* { return signature.get_type(); }
};

/**
 * @brief The current scope defined by blocks of code
 */
struct BlockScope {
    std::vector<std::shared_ptr<Variable>> vars;  //< variables in current scope (block)
};

/**
 * @brief a single frame of the call stack
 *
 * Call stack frames get pushed during a function call and popped right after it.
 * They hold a vector of function arguments as references to variables (temporary or other).
 * Block scopes are held here as well.
 */
struct CallStackFrame {
    std::vector<std::shared_ptr<VarRef>> args;  //< variables passed by reference
    std::vector<BlockScope> var_scope;          //< variables originating inside the function
};

/**
 * @brief enum representing where we got a result from (for determining whether to pass by value or reference)
 */
enum class ReceivedBy {
    EXPR,
    VAR,
};

/**
 * @brief a function argument, either a value or reference, depending on user input
 */
using Arg = std::variant<std::shared_ptr<Variable>, ValType>;

/**
 * @brief a vector of function arguments
 */
using ArgVector = std::vector<Arg>;
