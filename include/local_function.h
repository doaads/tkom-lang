#pragma once

#include <variant>

#include "function.h"

struct Variable;
struct VarRef;
class Callable;
class InterpreterVisitor;

using ValType =
    std::variant<std::monostate, std::string, int, double, bool, std::shared_ptr<Callable>>;

using Arg = std::variant<std::shared_ptr<Variable>, ValType>;
using ArgVector = std::vector<Arg>;

/**
 * @brief general class for registering available functions.
 */
class Callable {
   public:
    virtual ~Callable() = default;
    virtual void call(InterpreterVisitor& interpreter, ArgVector params) = 0;
    [[nodiscard]] virtual auto get_func() const -> const Function* = 0;
    [[nodiscard]] virtual auto get_type() const -> const Type* = 0;
    [[nodiscard]] virtual auto get_name() const -> const std::string = 0;
};

/**
 * @brief class for holding a direct reference to parsed functions
 */
class GlobalFunction : public Callable {
   private:
    std::unique_ptr<Type> type;
    const Function* func;

   public:
    GlobalFunction(const Function* func);
    void call(InterpreterVisitor& interpreter, ArgVector params) override;
    [[nodiscard]] auto get_func() const -> const Function* override;
    auto prepare_func_args(InterpreterVisitor& interpreter, ArgVector& args) const
        -> std::vector<std::shared_ptr<VarRef>>;
    [[nodiscard]] auto get_type() const -> const Type* override { return type.get(); }
    [[nodiscard]] auto get_name() const -> const std::string override {
        return func->get_signature()->get_name();
    }
};

/**
 * @brief class for holding decorated/bind fronted functions as variables
 *
 * Local functions differ from global functions only with their signature.
 */
class LocalFunction : public Callable {
   private:
    std::shared_ptr<Callable> callee;
    std::unique_ptr<Type> type;
    std::string name;
    ArgVector bound_args;

   public:
    LocalFunction(std::shared_ptr<Callable> callee_func, ArgVector bound_args);
    LocalFunction(std::unique_ptr<Type> type);
    void call(InterpreterVisitor& interpreter, ArgVector params) override;
    [[nodiscard]] auto get_func() const -> const Function* override;
    [[nodiscard]] auto get_type() const -> const Type* override { return type.get(); }
    [[nodiscard]] auto get_name() const -> const std::string override { return name; }
};
