#pragma once

#include <exception>

class CompilerException : public std::exception {
    public:
        CompilerException() = default;
        virtual const char* what() const noexcept override = 0;
        virtual ~CompilerException() = default;
};

class CompilerWarning : public CompilerException {
    public:
        CompilerWarning() = default;
        virtual ~CompilerWarning() = default;
};

class CompilerError : public CompilerException {
    public:
        CompilerError() = default;
        virtual ~CompilerError() = default;
};

class OverflowWarning : public CompilerWarning {
    public:
        OverflowWarning() = default;
        const char* what() const noexcept override;
        ~OverflowWarning() = default;
};

class UnexpectedToken : public CompilerError {
    public:
        UnexpectedToken() = default;
        const char* what() const noexcept override;
        ~UnexpectedToken() = default;
};
