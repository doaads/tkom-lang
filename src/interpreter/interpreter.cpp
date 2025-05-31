#include "interpreter.h"

#include <memory>
#include <utility>

#include "interpreter_shall.h"

InterpreterVisitor::InterpreterVisitor(std::vector<std::shared_ptr<Callable>> builtins)
    : functions(std::move(builtins)) {}

void InterpreterVisitor::visit(const Program& program) {
    for (const auto& fn : program.get_functions()) {
        register_function(fn);
    }
    auto main = find_func("main");
    main->call(*this, {});
}

void InterpreterVisitor::visit(const Function& func) {
    try_visit(func.get_body());
    if (returning) returning = false;
}

void InterpreterVisitor::visit(const Block& block) {
    call_stack.back().var_scope.emplace_back();
    for (auto& stmt : block.get_statements()) {
        try_visit(stmt);
        if (returning) break;
    }
    call_stack.back().var_scope.pop_back();
}

void InterpreterVisitor::visit(const CallExpr& expr) {
    ArgVector args;
    try_visit(expr.get_func_name());
    auto func = current_value;

    for (auto& arg : expr.get_args()) {
        try_visit(arg);
        // we got a identifier expression, pass by ref
        if (receiver == ReceivedBy::VAR) {
            auto var_ptr = var;
            args.emplace_back(var_ptr.lock());
            // we got a standard expression, we will pass by value
        } else {
            args.emplace_back(current_value);
        }
    }
    std::get<std::shared_ptr<Callable>>(func)->call(*this, args);
    receiver = ReceivedBy::EXPR;
}

void InterpreterVisitor::visit(const IdentifierExpr& expr) {
    std::weak_ptr<Variable> found = find_var_in_frame(expr.get_identifier());

    if (auto var_val = found.lock()) {
        current_value = var_val->value;
        var = found;
    } else {
        auto func = find_func(expr.get_identifier());
        shall(func, "Unknown identifier");
        current_value = func;
    }
    receiver = ReceivedBy::VAR;
}

void InterpreterVisitor::visit(const AssignStatement& stmt) {
    try_visit(stmt.get_value());
    ValType value = current_value;

    auto type = stmt.get_type();
    const std::string identifier = stmt.get_identifier();

    if (!type) {
        modify_var(identifier);
    } else {
        shall(!find_var_in_frame(identifier).lock(), "Variable " + identifier + " is already defined in this frame");
        register_var(*stmt.get_signature());
    }
}

void InterpreterVisitor::visit(const VarType& type) { current_type = type.get_type(); }

void InterpreterVisitor::visit(const FuncType& type) { current_type = &type; }

void InterpreterVisitor::visit(const WhileLoopStatement& stmt) {
    auto condition = stmt.get_condition();
    while (eval_condition(*condition)) {
        try_visit(stmt.get_body());
    }
}

void InterpreterVisitor::visit(const ConditionalStatement& stmt) {
    auto condition = stmt.get_condition();
    if (eval_condition(*condition)) {
        try_visit(stmt.get_body());
    } else if (auto else_st = stmt.get_else_st()) {
        try_visit(else_st);
    }
}

void InterpreterVisitor::visit(const ElseStatement& stmt) { try_visit(stmt.get_body()); }

void InterpreterVisitor::visit(const ForLoopStatement& stmt) {
    auto& args = *stmt.get_args();
    auto condition = args.condition.get();
    auto iterator = get_for_iterator(args);

    try_visit(stmt.get_on_iter());
    ValType on_iter = current_value;

    shall(std::holds_alternative<std::shared_ptr<Callable>>(on_iter), "on iter call must be a function");

    auto on_iter_func = std::get<std::shared_ptr<Callable>>(on_iter);
    ArgVector on_iter_arg = {iterator.lock()};

    for (;eval_condition(*condition); on_iter_func->call(*this, on_iter_arg)){
        try_visit(stmt.get_body());
    }
}

void InterpreterVisitor::visit(const RetStatement& stmt) { 
    try_visit(stmt.get_retval());
    returning = true;
}

void InterpreterVisitor::visit(const CallStatement& stmt) {
    try_visit(stmt.get_call());
}

void InterpreterVisitor::visit(const BindFrtExpr& expr) {
    // bindfrt can contain a decorator, we visit it first
    try_visit(expr.get_func_name());

    shall(std::holds_alternative<std::shared_ptr<Callable>>(current_value), "Expected valid function identifier");
    auto func = std::get<std::shared_ptr<Callable>>(current_value);

    // grab arguments
    // pass all by value for bindfront
    ArgVector args;
    for (auto& arg : expr.get_args()) {
        try_visit(arg);
        if (receiver == ReceivedBy::VAR) {
            auto var_ptr = var;
            args.emplace_back(var_ptr.lock()->value);
        } else {
            args.emplace_back(current_value);
        }
    }
    auto new_func = std::make_shared<LocalFunction>(func, args);
    current_type = new_func->get_type();
    current_value = std::move(new_func);
}
