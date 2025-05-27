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

/*
 * @brief: general class for registering available functions.
 */
class Callable {
   public:
    ~Callable() = default;
    virtual void call(InterpreterVisitor& interpreter,
                      ArgVector params) = 0;
    virtual const Function* get_func() const = 0;
};


/*
 * @brief: class for holding a direct reference to parsed functions
 */
class GlobalFunction : public Callable {
   private:
    const Function* func;

   public:
    GlobalFunction(const Function* func);
    void call(InterpreterVisitor& interpreter,
              ArgVector params) override;
    const Function* get_func() const override;
    std::vector<std::shared_ptr<VarRef>> prepare_func_args(InterpreterVisitor& interpreter, ArgVector& args) const;
};

/*
 * @brief: class for holding decorated/bind fronted functions as variables
 *
 * Local functions differ from global functions only with their signature.
 */
class LocalFunction : public Callable {
   private:
    std::shared_ptr<Callable> callee;
    ArgVector bound_args;

   public:
    LocalFunction(std::shared_ptr<Callable> callee, ArgVector bound_args);
    void call(InterpreterVisitor& interpreter,
              ArgVector params) override;
    const Function* get_func() const override;
};
