#pragma once

#include <exception>

#include "position.h"

class CompilerException : public std::exception {
   private:
    Position position;

   public:
    CompilerException() = default;
    virtual bool is_error() const = 0;
    virtual const char *what() const noexcept override = 0;
    virtual ~CompilerException() = default;
    void set_position(Position pos);
    const Position get_position() const;
};

class CompilerWarning : public CompilerException {
   public:
    CompilerWarning() = default;
    virtual ~CompilerWarning() = default;
    bool is_error() const override;
};

class CompilerError : public CompilerException {
   public:
    CompilerError() = default;
    virtual ~CompilerError() = default;
    bool is_error() const override;
};

class OverflowWarning : public CompilerWarning {
   public:
    OverflowWarning() = default;
    const char *what() const noexcept override;
    ~OverflowWarning() = default;
};

class IdentifierLengthExceeded : public CompilerWarning {
   public:
    IdentifierLengthExceeded() = default;
    const char *what() const noexcept override;
    ~IdentifierLengthExceeded() = default;
};

class UnexpectedToken : public CompilerError {
   public:
    UnexpectedToken() = default;
    const char *what() const noexcept override;
    ~UnexpectedToken() = default;
};

class UnterminatedString : public CompilerError {
   public:
    UnterminatedString() = default;
    const char *what() const noexcept override;
    ~UnterminatedString() = default;
};

class GeneralError : public std::runtime_error {
   private:
    Position pos;
    static std::string format_message(const Position &pos, const std::string &msg);

   public:
    GeneralError(const Position &pos, const std::string &msg);
    virtual ~GeneralError() = default;
    const Position get_position() const;
};

class ParserError : public GeneralError {
    public:
        ParserError(const Position& pos, const std::string &msg) : GeneralError(pos, msg) {}
};

class InterpreterError : public std::runtime_error {
    public:
        InterpreterError(const std::string &msg) : std::runtime_error(msg) {}
};
