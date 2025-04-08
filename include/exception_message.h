#pragma once

#include "exceptions.h"
#include "position.h"

class ExceptionHandler {
    public:
        ~ExceptionHandler() = default;
        ExceptionHandler() = default;
        void print_exception_message(CompilerException& e, Position position);
        bool is_error(CompilerException& e);
        bool is_warning(CompilerException& e);
};
