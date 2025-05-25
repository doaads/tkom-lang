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

struct Variable {
    const VariableSignature* signature;  // variable type and name as parsed by the parser
    ValType value;
    const Type* get_type() { return signature->get_type(); }
};

struct VarRef {
    std::weak_ptr<Variable> ref;
    std::string curr_name;
};

struct CallStackFrame {
    std::vector<std::shared_ptr<VarRef>> args;   // passed by reference
    std::vector<std::shared_ptr<Variable>> vars; // originating inside the function
};

class InterpreterVisitor : public Visitor {
    private:
        ValType current_value;
        std::string current_type;
        std::vector<ValType> call_args;

        std::vector<std::shared_ptr<GlobalFunction>> functions;
        ValType decorate(ValType decorator, ValType decoratee);
        void register_function(const Function* func);
        std::vector<CallStackFrame> call_stack;

        ValType add(ValType lhs, ValType rhs);
        ValType sub(ValType lhs, ValType rhs);
        ValType mul(ValType lhs, ValType rhs);
        ValType div(ValType lhs, ValType rhs);

        template <typename CompareFunc>
        ValType compare(ValType lhs, ValType rhs, CompareFunc func);

        template <typename UnaryFunc>
        ValType unary(ValType val, UnaryFunc func);

        template <typename LogicalFunc>
        ValType logical(ValType lhs, ValType rhs, LogicalFunc func);

    public:
        void visit(const Program &program);

        void visit(const LiteralExpr &expr);
        void visit(const IdentifierExpr &expr);
        void visit(const UnaryExpr &expr);
        void visit(const BinaryExpr &expr);
        void visit(const CallExpr &expr);
        void visit(const BindFrtExpr &expr) {(void) expr; return;}

        void visit(const ForLoopStatement &stmt) {(void)stmt; return;}
        void visit(const WhileLoopStatement &stmt) {(void)stmt; return;}
        void visit(const ConditionalStatement &stmt) {(void)stmt; return;}
        void visit(const ElseStatement &stmt) {(void)stmt; return;}
        void visit(const RetStatement &stmt) {(void)stmt; return;}
        void visit(const CallStatement &stmt) {(void)stmt; return;}
        void visit(const AssignStatement &stmt);

        void visit(const Block &block);
        void visit(const VarType &var) {(void)var; return;}
        void visit(const FuncType &func) {(void)func; return;}

        void visit(const VariableSignature &var) {(void)var; return;}
        void visit(const FuncSignature &sign) {(void)sign; return;}
        void visit(const Function &func);

        ValType get_value() const;
        void push_call_stack(CallStackFrame frame);
        void pop_call_stack();

        std::weak_ptr<Variable> find_var_in_frame(const std::string& name);
        std::shared_ptr<Callable> find_func(const std::string& name);
};

