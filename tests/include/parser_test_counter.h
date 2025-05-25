#pragma once

#include "function.h"
#include "visitor.h"
#include "type.h"
#include "variable.h"
#include "statement_specific.h"

#pragma once

class ParserTestCounter : public Visitor {
public:
    // Expression counters
    int literal_expr_count = 0;
    int identifier_expr_count = 0;
    int unary_expr_count = 0;
    int binary_expr_count = 0;
    int call_expr_count = 0;
    int bind_frt_expr_count = 0;

    // Statement counters
    int for_stmt_count = 0;
    int while_stmt_count = 0;
    int conditional_stmt_count = 0;
    int else_stmt_count = 0;
    int ret_stmt_count = 0;
    int call_stmt_count = 0;
    int assign_stmt_count = 0;

    // Other
    int block_count = 0;
    int var_type_count = 0;
    int func_type_count = 0;
    int void_count = 0;

    int func_param_count = 0;
    int func_signature_count = 0;
    int function_count = 0;

    void visit(const Program& prog) override {
        for (const auto& func : prog.get_functions()) {
            func->accept(*this);
        }
    }

    // Expression visitors
    void visit(const LiteralExpr&) override { 
        literal_expr_count++; 
    }

    void visit(const IdentifierExpr&) override {
        identifier_expr_count++;
    }

    void visit(const UnaryExpr& expr) override {
        unary_expr_count++;
        expr.get_right()->accept(*this);
    }

    void visit(const BinaryExpr& expr) override {
        binary_expr_count++;
        expr.get_left()->accept(*this);
        expr.get_right()->accept(*this);
    }

    void visit(const CallExpr& expr) override { 
        call_expr_count++;
        expr.get_func_name()->accept(*this);
        for (const auto& arg : expr.get_args()) {
            arg->accept(*this);
        }
    }

    void visit(const BindFrtExpr& expr) override {
        bind_frt_expr_count++;
        expr.get_func_name()->accept(*this);
        for (auto& arg : expr.get_args()) {
            arg->accept(*this);
        }
    }

    // Statement visitors
    void visit(const ForLoopStatement& stmt) override {
        for_stmt_count++;
        stmt.get_body()->accept(*this);
        stmt.get_on_iter()->accept(*this);
    }

    void visit(const WhileLoopStatement& stmt) override {
        while_stmt_count++;
        stmt.get_condition()->accept(*this);
        stmt.get_body()->accept(*this);
    }

    void visit(const ConditionalStatement& stmt) override {
        conditional_stmt_count++;
        stmt.get_condition()->accept(*this);
        stmt.get_body()->accept(*this);
        if (const Statement* else_st = stmt.get_else_st())
            else_st->accept(*this);
    }

    void visit(const ElseStatement& stmt) override {
        else_stmt_count++;
        stmt.get_body()->accept(*this);
    }

    void visit(const RetStatement& stmt) override {
        ret_stmt_count++;

        if (auto retval = stmt.get_retval())
            retval->accept(*this);
    }

    void visit(const CallStatement& stmt) override {
        call_stmt_count++;
        stmt.get_call()->accept(*this);
    }

    void visit(const AssignStatement& stmt) override {
        assign_stmt_count++;
        stmt.get_value()->accept(*this);
        stmt.get_type()->accept(*this);
        identifier_expr_count++;
    }

    // Other visitors
    void visit(const Block& block) override {
        block_count++;
        for (auto& stmt : block.get_statements()) {
            stmt->accept(*this);
        }
    }
    void visit(const VarType&) override { var_type_count++; }
    void visit(const FuncType&) override { func_type_count++; }

    void visit(const VariableSignature& var) override {
        func_param_count++;
        var.get_type()->accept(*this);
    }

    void visit(const FuncSignature& sign) override {
        func_signature_count++;
        if (const Type* ret_type = sign.get_type()) {
            ret_type->accept(*this);
        } else {
            void_count++;
        }
        for (auto& arg : sign.get_params()) {
            arg->accept(*this);
        }
    }
    void visit(const Function& func) override {
        function_count++;
        func.get_signature()->accept(*this);
        func.get_body()->accept(*this);
    }

    // Optional reset if reusing visitor
    void reset() {
        *this = ParserTestCounter{};
    }
};

