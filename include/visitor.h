#pragma once

#include <ostream>

#include "block.h"
#include "expression.h"
#include "function.h"
#include "program.h"
#include "statement.h"
#include "statement_specific.h"
#include "variable.h"

/**
 * @brief Abstract base class for visitors that can traverse different program constructs.
 *
 * The visitor pattern allows actions to be applied to different types of program nodes
 * without modifying their class structures.
 */
class Visitor {
   public:
    virtual ~Visitor() = default;

    virtual void visit(const Program &program) = 0;

    virtual void visit(const LiteralExpr &expr) = 0;
    virtual void visit(const IdentifierExpr &expr) = 0;
    virtual void visit(const UnaryExpr &expr) = 0;
    virtual void visit(const BinaryExpr &expr) = 0;
    virtual void visit(const CallExpr &expr) = 0;
    virtual void visit(const BindFrtExpr &expr) = 0;

    virtual void visit(const ForLoopStatement &stmt) = 0;
    virtual void visit(const WhileLoopStatement &stmt) = 0;
    virtual void visit(const ConditionalStatement &stmt) = 0;
    virtual void visit(const ElseStatement &stmt) = 0;
    virtual void visit(const RetStatement &stmt) = 0;
    virtual void visit(const CallStatement &stmt) = 0;
    virtual void visit(const AssignStatement &stmt) = 0;

    virtual void visit(const Block &block) = 0;
    virtual void visit(const VarType &var) = 0;
    virtual void visit(const FuncType &func) = 0;

    virtual void visit(const FuncParam &var) = 0;
    virtual void visit(const FuncSignature &sign) = 0;
    virtual void visit(const Function &func) = 0;
};

/**
 * @brief A concrete implementation of the Visitor that prints program nodes.
 *
 * This class implements the visit methods to output the structure of the program to
 * an output stream. It supports indentation for readability.
 */
class ParserPrinter : public Visitor {
   private:
    std::ostream &os;  ///< Output stream to print to.
    int indent;        ///< Current indentation level.

    /**
     * @brief Helper function to get the current indentation as a string.
     *
     * @return String representing the current indentation.
     */
    std::string indent_str() const;

    void increase_indent();
    void decrease_indent();

    /**
     * @brief Prints the position of a node.
     *
     * @param expr The node to print the position for.
     */
    void print_pos(const Node &expr);

   public:
    /**
     * @brief Constructs a ParserPrinter.
     *
     * @param os The output stream to print to.
     * @param indent The initial indentation level (default is 0).
     */
    ParserPrinter(std::ostream &os, int indent = 0) : os(os), indent(indent) {}
    ~ParserPrinter() = default;

    virtual void visit(const Program &program) override;

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
    void visit(const VarType &type) override;
    void visit(const FuncType &type) override;

    void visit(const FuncParam &var) override;
    void visit(const FuncSignature &sign) override;
    void visit(const Function &func) override;
};
