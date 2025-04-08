#include "exception_message.h"
#include <iostream>

void ExceptionHandler::print_exception_message(
        CompilerException& e,
        Position position) {
    if (is_warning(e)) {
        std::cout << "\033[1;33mWARNING: \033[0m";
    } else if (is_error(e)){
        std::cout << "\033[1;31mERROR: \033[0m";
    } else {
        std::cout << "COMPILER EXCEPTION: ";
    }

    std::cout << e.what() << " at " << position << std::endl;
}

bool ExceptionHandler::is_error(CompilerException& e) {
    return dynamic_cast<CompilerError*>(&e);
}

bool ExceptionHandler::is_warning(CompilerException& e) {
    return dynamic_cast<CompilerWarning*>(&e);
}
