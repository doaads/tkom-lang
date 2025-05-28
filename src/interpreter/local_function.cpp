#include "arithmetics.h"
#include "interpreter.h"
#include "local_function.h"
#include "type_cast.h"

void expect_arg_match(std::vector<const Type*>& expected, std::vector<const Type*>& received) {
    if (expected.size() != received.size())
        throw std::runtime_error("Invalid argument vector size");

    for (size_t i = 0; i < expected.size(); ++i) {
        if (!expected[i]->is_equal_to(received[i])) throw std::runtime_error("Type mismatch");
    }
}

GlobalFunction::GlobalFunction(const Function* func) : func(func) {}

std::vector<std::shared_ptr<VarRef>> GlobalFunction::prepare_func_args(InterpreterVisitor& interpreter, ArgVector& args) const {
    const auto expected = func->get_signature()->get_params();
    std::vector<std::shared_ptr<VarRef>> var_refs;
    for (size_t i = 0; i < expected.size(); ++i) {
        auto expected_arg = expected[i];
        std::visit(
            Overload{[&](std::shared_ptr<Variable> var) {
                         if (!expected_arg->get_type()->is_equal_to(var->get_type()))
                             throw std::runtime_error("Type mismatch");
                         var_refs.push_back(std::make_shared<VarRef>(var, expected[i]->get_name()));
                     },
                     [&](ValType value) {
                         auto type = expected[i]->get_type();
                         value = std::visit(TypeCast(), value, interpreter.init_var(*type));
                         auto temp_var = std::make_shared<Variable>(*expected[i], value);
                         var_refs.push_back(
                             std::make_shared<VarRef>(temp_var, expected[i]->get_name()));
                     }},
            args[i]);
    }
    return var_refs;
}

void GlobalFunction::call(InterpreterVisitor& interpreter, ArgVector args) {

    // turn non-referenced args into temporary variables, and verify type integrity
    auto prepared_args = prepare_func_args(interpreter, args);

    CallStackFrame frame = CallStackFrame(prepared_args, std::vector<BlockScope>());

    // push new call stack frame
    interpreter.push_call_stack(frame);

    // proper call
    func->accept(interpreter);

    // dispose of the call stack frame
    interpreter.pop_call_stack();
}

const Function* GlobalFunction::get_func() const { return func; }

LocalFunction::LocalFunction(std::shared_ptr<Callable> callee_func,
                             ArgVector bound_args)
    : callee(std::move(callee_func)), bound_args(bound_args) {
        // adjust this function's type
        type = callee->get_type()->clone(bound_args.size());
    }

/*
 * @brief: bind arguments and call
 */
void LocalFunction::call(InterpreterVisitor& interpreter, ArgVector args) {
    if (callee == nullptr) throw std::runtime_error("No function to call");

    ArgVector full_arg_list = bound_args;
    full_arg_list.insert(full_arg_list.end(), args.begin(), args.end());

    callee->call(interpreter, full_arg_list);

    return;
}

const Function* LocalFunction::get_func() const {
    if (callee == nullptr) return nullptr;
    return callee->get_func();
}

