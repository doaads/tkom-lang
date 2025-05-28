#pragma once
#include <functional>
#include "local_function.h"
#include "interpreter.h"
#include "type.h"


using builtin = std::function<void(ArgVector&)>;

class BuiltIn : public Callable {
private:
    builtin func;
    std::string name;
    std::unique_ptr<Type> type;

public:
    BuiltIn(builtin f, const std::string& name, std::unique_ptr<Type> t)
      : func(std::move(f)), name(name), type(std::move(t)) {}

    void call(InterpreterVisitor&, ArgVector params) override {
        func(params);
    }

    const Function* get_func() const override {
        throw std::runtime_error("Cannot retrieve builtin func");
    }
    const Type* get_type() const override { return type.get(); }
    const std::string get_name() const override { return name; }
};

