#pragma once

#include <variant>

#include "function.h"

struct Variable;
struct VarRef;
class Callable;
class InterpreterVisitor;

using ValType =
    std::variant<std::monostate, std::string, int, double, bool, std::shared_ptr<Callable>>;

/*
 * @brief: general class for registering available functions.
 */
class Callable {
   public:
    ~Callable() = default;
    virtual void call(InterpreterVisitor& interpreter,
                      std::vector<std::shared_ptr<VarRef>>& params) = 0;
    virtual const Function* get_func() const = 0;
};

using ValType =
    std::variant<std::monostate, std::string, int, double, bool, std::shared_ptr<Callable>>;

/*
 * @brief: class for holding a direct reference to parsed functions
 */
class GlobalFunction : public Callable {
   private:
    const Function* func;

   public:
    GlobalFunction(const Function* func);
    void call(InterpreterVisitor& interpreter,
              std::vector<std::shared_ptr<VarRef>>& params) override;
    const Function* get_func() const override;
};

/*
 * @brief: class for holding decorated/bind fronted functions as variables
 *
 * Local functions differ from global functions only with their signature.
 */
class LocalFunction : public Callable {
   private:
    std::shared_ptr<Callable> callee;
    std::vector<std::shared_ptr<VarRef>> bound_args;

   public:
    LocalFunction(std::shared_ptr<Callable> callee, std::vector<std::shared_ptr<VarRef>> bound_args);
    void call(InterpreterVisitor& interpreter,
              std::vector<std::shared_ptr<VarRef>>& params) override;
    const Function* get_func() const override;
};

/*   EXAMPLE:
 *
 *   int decorator :: [int::int] func, int a {
 *       ...
 *   }

 *   int some_func :: int a {
 *       ...
 *   }

 *   some_func @ decorator => [int::int] decorated_func;

 *   we get:
 *      callee = decorator (GlobalFunction)
 *      bound_args = {std::shared_ptr<Callable> some_func} (also GlobalFunction)
 */
