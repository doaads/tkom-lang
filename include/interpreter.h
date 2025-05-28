#pragma once

#include "exceptions.h"
#include "local_function.h"
#include "visitor.h"

using ValType = std::variant<
    std::monostate,
    std::string,
    int,
    double,
    bool,
    std::shared_ptr<Callable>
>;

using TypeType = std::variant<
    const Type*,
    BaseType
>;

struct VarRef {
    std::shared_ptr<Variable> ref;  //< variable reference
    std::string curr_name;  //< name in current scope
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
    std::vector<std::shared_ptr<VarRef>> args;   //< variables passed by reference
    std::vector<BlockScope> var_scope; //< variables originating inside the function
};

enum class ReceivedBy {
    EXPR,
    VAR,
};

using Arg = std::variant<std::shared_ptr<Variable>, ValType>;
using ArgVector = std::vector<Arg>;

/**
 * @brief Interpreter implementation
 *
 * The interpreter goes through the parsed synatx tree and ensures the following:
 *      - strong assignment - the variable's type cannot be changed as runtime
 *      - weak typing - implicit casting
 *      - passing variables as a reference
 */
class InterpreterVisitor : public Visitor {
    private:
        ValType current_value;  //< value returned by an expression/function/identifier
        TypeType current_type;  //< currently interpreted type
        std::weak_ptr<Variable> var;  //< variable pointer, received from an identifier

        ReceivedBy receiver;  //< indicator of where we got current_value from
        bool returning = false;  //< indicator telling us whether we are currently returning from a function

        std::vector<std::shared_ptr<Callable>> functions;  //< vector of available global functions (either user-defined or built-in)
        ValType decorate(ValType decorator, ValType decoratee);

        /**
         * @brief register a new global function
         */
        void register_function(const Function* func);

        std::vector<CallStackFrame> call_stack; //<  the call stack, growing with each function call

        /**
         * @brief register a new variable with a given signature
         */
        void register_var(const VariableSignature& signature);

        /**
         * @brief assign the current value to a variable with the given identifier
         */
        void modify_var(const std::string& identifier);

        /**
         * @brief prepare a for loop iterator
         */
        std::weak_ptr<Variable> get_for_iterator(const ForLoopArgs& args);

        /**
         * @brief evaluate a condition
         */
        bool eval_condition(const Expression& expr);

        template <typename T>
        void try_visit(T obj) {
            try {
                obj->accept(*this);
            } catch (InterpreterError& e) {
                throw GeneralError(obj->get_position(), e.what());
            }
        }
    public:
        InterpreterVisitor() = default;

        /**
         * @brief construct with a vector of builtin functions
         */
        InterpreterVisitor(std::vector<std::shared_ptr<Callable>> builtins);
        void visit(const Program &program);

        void visit(const LiteralExpr &expr);
        void visit(const IdentifierExpr &expr);
        void visit(const UnaryExpr &expr);
        void visit(const BinaryExpr &expr);
        void visit(const CallExpr &expr);
        void visit(const BindFrtExpr &expr);

        void visit(const ForLoopStatement &stmt);
        void visit(const WhileLoopStatement &stmt);
        void visit(const ConditionalStatement &stmt);
        void visit(const ElseStatement &stmt);
        void visit(const RetStatement &stmt);
        void visit(const CallStatement &stmt);
        void visit(const AssignStatement &stmt);

        void visit(const Block &block);
        void visit(const VarType &var);
        void visit(const FuncType &func);

        void visit(const VariableSignature &var) {(void)var; return;}
        void visit(const FuncSignature &sign) {(void)sign; return;}
        void visit(const Function &func);

        /**
         * @brief return the current value
         */
        ValType get_value() const;

        /**
         * @brief override the current value
         */
        void override_value(ValType val);

        /**
         * @brief push a new call stack frame into the stack
         */
        void push_call_stack(CallStackFrame frame);

        /**
         * @brief pop the call stack
         */
        void pop_call_stack();

        /**
         * @brief get a variable reference in the current frame by identifier
         */
        std::weak_ptr<Variable> find_var_in_frame(const std::string& name);

        /**
         * @brief find a global function with a given name
         */
        std::shared_ptr<Callable> find_func(const std::string& name);

        /**
         * initialize a variable with a given type
         */
        ValType init_var(const Type& type);
};

