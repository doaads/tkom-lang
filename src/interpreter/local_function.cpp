#include "arithmetics.h"
#include "interpreter.h"
#include "interpreter_shall.h"
#include "local_function.h"
#include "type_cast.h"

GlobalFunction::GlobalFunction(const Function* func) : func(func) {
    type = func->get_signature()->clone_type_as_type_obj();
}

auto GlobalFunction::prepare_func_args(InterpreterVisitor& interpreter, ArgVector& args) const
    -> std::vector<std::shared_ptr<VarRef>> {
    const auto expected = func->get_signature()->get_params();
    shall(expected.size() == args.size(), "Invalid argument vector size");
    std::vector<std::shared_ptr<VarRef>> var_refs;
    for (size_t i = 0; i < expected.size(); ++i) {
        auto expected_arg = expected[i];
        std::visit(
            Overload{
                [&](std::shared_ptr<Variable> var) {
                    shall(expected_arg->get_type()->is_equal_to(var->get_type()), "Type mismatch");
                    var_refs.push_back(std::make_shared<VarRef>(var, expected[i]->get_name()));
                },
                [&](ValType value) {
                    auto type = expected[i]->get_type();
                    value = std::visit(TypeCast(), value, interpreter.init_var(*type));
                    auto temp_var = std::make_shared<Variable>(*expected[i], value);
                    var_refs.push_back(std::make_shared<VarRef>(temp_var, expected[i]->get_name()));
                }},
            args[i]);
    }
    return var_refs;
}

/**
 * @brief verify argument types, push call frame and call
 */
void GlobalFunction::call(InterpreterVisitor& interpreter, ArgVector args) {
    // turn non-referenced args into temporary variables, and verify type integrity
    auto prepared_args = prepare_func_args(interpreter, args);

    CallStackFrame frame = CallStackFrame(prepared_args, std::vector<BlockScope>());

    // push new call stack frame
    interpreter.push_call_stack(frame);

    // proper call
    func->accept(interpreter);

    // verify the received type
    auto current_val = interpreter.get_value();
    auto ret_type = get_type()->get_ret_type();
    if (!ret_type->is_equal_to(std::make_unique<VarType>(BaseType::VOID, false).get())) {
        auto retval = interpreter.init_var(*ret_type);
        // cast the value into desired return type
        interpreter.override_value(std::visit(TypeCast(), current_val, retval));
    }

    // dispose of the call stack frame
    interpreter.pop_call_stack();
}

auto GlobalFunction::get_func() const -> const Function* { return func; }

LocalFunction::LocalFunction(std::shared_ptr<Callable> callee_func, ArgVector bound_args)
    : callee(std::move(callee_func)), bound_args(bound_args) {
    // adjust this function's type
    type = callee->get_type()->clone(bound_args.size());
}

LocalFunction::LocalFunction(std::unique_ptr<Type> type) : type(std::move(type)) {}

/**
 * @brief bind arguments and call
 */
void LocalFunction::call(InterpreterVisitor& interpreter, ArgVector args) {
    shall(callee, "No function to call");

    ArgVector full_arg_list = bound_args;
    full_arg_list.insert(full_arg_list.end(), args.begin(), args.end());

    callee->call(interpreter, full_arg_list);

    return;
}

auto LocalFunction::get_func() const -> const Function* {
    if (callee == nullptr) return nullptr;
    return callee->get_func();
}
