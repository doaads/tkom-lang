#pragma once

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
    std::shared_ptr<Variable> ref;
    std::string curr_name;
};

struct Variable {
    const VariableSignature& signature;  // variable type and name as parsed by the parser
    ValType value;
    const Type* get_type() { return signature.get_type(); }
};

struct BlockScope {
    std::vector<std::shared_ptr<Variable>> vars;
};

struct CallStackFrame {
    std::vector<std::shared_ptr<VarRef>> args;   // passed by reference
    std::vector<BlockScope> var_scope; // originating inside the function
};

enum class ReceivedBy {
    EXPR,
    VAR,
};

using Arg = std::variant<std::shared_ptr<Variable>, ValType>;
using ArgVector = std::vector<Arg>;

class InterpreterVisitor : public Visitor {
    private:
        ValType current_value;
        TypeType current_type;
        std::weak_ptr<Variable> var;
        std::vector<ValType> call_args;

        ReceivedBy receiver;
        bool returning = false;

        std::vector<std::shared_ptr<GlobalFunction>> functions;
        ValType decorate(ValType decorator, ValType decoratee);
        void register_function(const Function* func);
        std::vector<CallStackFrame> call_stack;

        void register_var(const VariableSignature& signature);
        void modify_var(const std::string& identifier);
        std::weak_ptr<Variable> get_for_iterator(const ForLoopArgs& args);

        bool eval_condition(const Expression& expr);
    public:
        void visit(const Program &program);

        void visit(const LiteralExpr &expr);
        void visit(const IdentifierExpr &expr);
        void visit(const UnaryExpr &expr);
        void visit(const BinaryExpr &expr);
        void visit(const CallExpr &expr);
        void visit(const BindFrtExpr &expr) {(void) expr; return;}

        void visit(const ForLoopStatement &stmt);
        void visit(const WhileLoopStatement &stmt);
        void visit(const ConditionalStatement &stmt);
        void visit(const ElseStatement &stmt);
        void visit(const RetStatement &stmt);
        void visit(const CallStatement &stmt);
        void visit(const AssignStatement &stmt);

        void visit(const Block &block);
        void visit(const VarType &var);
        void visit(const FuncType &func) {(void)func; return;}

        void visit(const VariableSignature &var) {(void)var; return;}
        void visit(const FuncSignature &sign) {(void)sign; return;}
        void visit(const Function &func);

        ValType get_value() const;
        void push_call_stack(CallStackFrame frame);
        void pop_call_stack();

        std::weak_ptr<Variable> find_var_in_frame(const std::string& name);
        std::shared_ptr<Callable> find_func(const std::string& name);
        ValType init_var(const Type& type);
};

