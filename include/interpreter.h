#pragma once

#include "exceptions.h"
#include "interpreter_helpers.h"
#include "local_function.h"
#include "visitor.h"

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
    ValType current_value;        //< value returned by an expression/function/identifier
    TypeType current_type;        //< currently interpreted type
    std::weak_ptr<Variable> var;  //< variable pointer, received from an identifier

    ReceivedBy receiver;  //< indicator of where we got current_value from
    bool returning =
        false;  //< indicator telling us whether we are currently returning from a function

    std::vector<std::shared_ptr<Callable>>
        functions;  //< vector of available global functions (either user-defined or built-in)
    auto decorate(ValType decorator, ValType decoratee) -> ValType;

    /**
     * @brief register a new global function
     */
    void register_function(const Function *func);

    std::vector<CallStackFrame> call_stack;  //<  the call stack, growing with each function call

    /**
     * @brief register a new variable with a given signature
     */
    void register_var(const VariableSignature &signature);

    /**
     * @brief assign the current value to a variable with the given identifier
     */
    void modify_var(const std::string &identifier);

    /**
     * @brief prepare a for loop iterator
     */
    auto get_for_iterator(const ForLoopArgs &args) -> std::weak_ptr<Variable>;

    /**
     * @brief evaluate a condition
     */
    auto eval_condition(const Expression &expr) -> bool;

    template <typename T>
    void try_visit(T obj) {
        try {
            obj->accept(*this);
        } catch (InterpreterError &e) {
            throw GeneralError(obj->get_position(), e.what());
        }
    }

   public:
    InterpreterVisitor() = default;

    /**
     * @brief construct with a vector of builtin functions
     */
    InterpreterVisitor(std::vector<std::shared_ptr<Callable>> builtins);
    void visit(const Program &program) override;

    void visit(const LiteralExpr &expr) override;
    void visit(const IdentifierExpr &expr) override;
    void visit(const UnaryExpr &expr) override;
    void visit(const BinaryExpr &expr) override;
    void visit(const CallExpr &expr) override;
    void visit(const BindFrtExpr &expr) override;

    void visit(const ForLoopStatement &stmt) override;
    void visit(const WhileLoopStatement &stmt) override;
    void visit(const ConditionalStatement &stmt) override;
    void visit(const ElseStatement &stmt) override;
    void visit(const RetStatement &stmt) override;
    void visit(const CallStatement &stmt) override;
    void visit(const AssignStatement &stmt) override;

    void visit(const Block &block) override;
    void visit(const VarType &var) override;
    void visit(const FuncType &func) override;

    void visit(const VariableSignature &var) override {
        (void)var;
        return;
    }
    void visit(const FuncSignature &sign) override {
        (void)sign;
        return;
    }
    void visit(const Function &func) override;

    /**
     * @brief return the current value
     */
    [[nodiscard]] auto get_value() const -> ValType;

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
    auto find_var_in_frame(const std::string &name) -> std::weak_ptr<Variable>;

    /**
     * @brief find a global function with a given name
     */
    auto find_func(const std::string &name) -> std::shared_ptr<Callable>;

    /**
     * initialize a variable with a given type
     */
    auto init_var(const Type &type) -> ValType;
};
