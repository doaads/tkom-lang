#pragma once

#include <memory>
#include "interpreter.h"
#include "parser.h"

/**
 * @brief small enum representing the input stream
 */
enum class From {
    STRING = 0,
    FILE
};

/**
 * @brief the vector of builtin functions
 */
using BuiltinVector = std::vector<std::shared_ptr<Callable>>;

/**
 * @brief a class representing the entire interpreter stack
 */
class TKOMInterpreter {
    private:
        const std::string filename;  //< filename of the source document
        const From from;  //< input source type
        std::unique_ptr<Program> program;  //< the parsed program
        std::shared_ptr<Parser> parser;  //< the parser
        bool verbose = false;  //< whether we want verbose logging

        InterpreterVisitor interpreter;  //< the interpreter

    public:
        /**
         * @brief construct the interpreter and initialize its components
         *
         * From::FILE will be assumed
         */
        TKOMInterpreter(const std::string& filename, BuiltinVector builtins, bool verbose = false);

        /**
         * @brief construct the interpreter and initialize its components
         *
         * Specify the input source
         */
        TKOMInterpreter(const std::string& program, From from, BuiltinVector builtins, bool verbose = false);
        auto process() -> int;
};
