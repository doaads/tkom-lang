#include <fstream>
#include <sstream>

#include "tkom_interpreter.h"
#include "print_error.h"

TKOMInterpreter::TKOMInterpreter(const std::string& filename, BuiltinVector builtins) : filename(filename), from(From::FILE) {
    std::shared_ptr<std::fstream> input =
        std::make_shared<std::fstream>(filename, std::fstream::in);
    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(input);
    parser = std::make_shared<Parser>(std::move(lexer));
    interpreter = InterpreterVisitor(std::move(builtins));
}

TKOMInterpreter::TKOMInterpreter(const std::string& program, From from, BuiltinVector builtins) : filename(program), from(from) {
    std::shared_ptr<std::istream> input;
    switch (from) {
        case From::STRING:
            input = std::make_unique<std::stringstream>(program);
            break;
        case From::FILE:
            input = std::make_unique<std::fstream>(program, std::fstream::in);
            break;
    }
    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(input);
    parser = std::make_shared<Parser>(std::move(lexer));
    interpreter = InterpreterVisitor(std::move(builtins));
}

int TKOMInterpreter::process() {
        if (!parser) throw ParserError(Position(0, 0), "Uninitialized Parser");

        ValType ret_code;
        try {
            program = parser->parse();
            program->accept(interpreter);
            ret_code = interpreter.get_value();
            if (!std::holds_alternative<int>(ret_code)) {
                throw InterpreterError("main must return an integer value");
            }

        } catch (const GeneralError &e) {
            std::cerr << "\033[1;31mError:\033[0m " << e.what() << std::endl;

            if (from == From::FILE) {
                print_error(e, filename);
            }
            return 1;
        } catch (const CompilerError &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        } catch (const std::exception &e) {
            std::cerr << "\033[1;31mUnhandled Error:\033[0m " << e.what() << std::endl;
            return 1;
        }


        return std::get<int>(ret_code);
}
