#include "interpreter.h"
#include "local_function.h"

void expect_arg_match(std::vector<const Type*>& expected, std::vector<const Type*>& received) {
    if (expected.size() != received.size())
        throw std::runtime_error("Invalid argument vector size");

    for (size_t i = 0; i < expected.size(); ++i) {
        if (!expected[i]->is_equal_to(received[i])) throw std::runtime_error("Type mismatch");
    }
}

GlobalFunction::GlobalFunction(const Function* func) : func(func) {}

void GlobalFunction::call(InterpreterVisitor& interpreter,
                          std::vector<std::shared_ptr<VarRef>>& args) {
    // set up call stack frame
    const auto expected = func->get_signature()->get_params();
    std::vector<const Type*> arg_types;
    for (auto& arg_ref : args) {
        auto weak_arg = arg_ref->ref;
        if (auto arg = weak_arg.lock()) arg_types.push_back(arg->get_type());  // fix here
    }
    std::vector<const Type*> expected_types;
    for (auto& arg : expected) {
        expected_types.push_back(arg->get_type());
    }

    expect_arg_match(arg_types, expected_types);

    CallStackFrame frame = CallStackFrame(args, std::vector<BlockScope>());

    // push new call stack frame
    interpreter.push_call_stack(frame);

    // proper call
    func->accept(interpreter);

    // dispose of the call stack frame
    interpreter.pop_call_stack();
}

const Function* GlobalFunction::get_func() const { return func; }

LocalFunction::LocalFunction(std::shared_ptr<Callable> callee,
                             std::vector<std::shared_ptr<VarRef>> bound_args)
    : callee(std::move(callee)), bound_args(bound_args) {}


/*
 * @brief: bind arguments and call
 */
void LocalFunction::call(InterpreterVisitor& interpreter,
                         std::vector<std::shared_ptr<VarRef>>& args) {
    if (callee == nullptr) throw std::runtime_error("No function to call");

    std::vector<std::shared_ptr<VarRef>> full_arg_list = bound_args;
    full_arg_list.insert(full_arg_list.end(), args.begin(), args.end());

    callee->call(interpreter, args);

    return;
}

const Function* LocalFunction::get_func() const {
    if (callee == nullptr) return nullptr;
    return callee->get_func();
}
