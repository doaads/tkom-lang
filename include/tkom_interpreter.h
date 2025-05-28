#pragma once

#include <memory>
#include "interpreter.h"
#include "parser.h"

enum class From {
    STRING = 0,
    FILE
};

using BuiltinVector = std::vector<std::shared_ptr<Callable>>;

class TKOMInterpreter {
    private:
        const std::string filename;
        const From from;
        std::unique_ptr<Program> program;
        std::shared_ptr<Parser> parser;

        InterpreterVisitor interpreter;

    public:
        TKOMInterpreter(const std::string& filename, BuiltinVector builtins);
        TKOMInterpreter(const std::string& program, From from, BuiltinVector builtins);
        int process();

};
