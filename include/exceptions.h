/**
 * @file exceptions.h
 *
 * Definitions for all compiler-related exceptions
 */

#pragma once
#include <exception>

#include "position.h"

/**
 * @brief A general compiler exception - thrown by the lexer
 */
class CompilerException : public std::exception {
   private:
    Position position;

   public:
    CompilerException() = default;
    [[nodiscard]] virtual auto is_error() const -> bool = 0;
    [[nodiscard]] auto what() const noexcept -> const char * override = 0;
    ~CompilerException() override = default;
    void set_position(Position pos);
    [[nodiscard]] auto get_position() const -> const Position;
};

/**
 * @brief A general compiler warning - thrown by the lexer
 */
class CompilerWarning : public CompilerException {
   public:
    CompilerWarning() = default;
    ~CompilerWarning() override = default;
    [[nodiscard]] auto is_error() const -> bool override;
};

/**
 * @brief A general compiler error - thrown by the lexer
 */
class CompilerError : public CompilerException {
   public:
    CompilerError() = default;
    ~CompilerError() override = default;
    [[nodiscard]] auto is_error() const -> bool override;
};

/**
 * @brief Thrown by the lexer when an integer/float exceeds its numeric limit limit
 */
class OverflowWarning : public CompilerWarning {
   public:
    OverflowWarning() = default;
    [[nodiscard]] auto what() const noexcept -> const char * override;
    ~OverflowWarning() override = default;
};

/**
 * @brief Thrown by the lexer when an integer's maximum length is exceeded
 */
class IdentifierLengthExceeded : public CompilerWarning {
   public:
    IdentifierLengthExceeded() = default;
    [[nodiscard]] auto what() const noexcept -> const char * override;
    ~IdentifierLengthExceeded() override = default;
};

/**
 * @brief Thrown by the lexer when an illegal character appears in the token
 */
class UnexpectedToken : public CompilerError {
   public:
    UnexpectedToken() = default;
    [[nodiscard]] auto what() const noexcept -> const char * override;
    ~UnexpectedToken() override = default;
};

/**
 * @brief Thrown by the lexer when a newline is detected before a string's end
 */
class UnterminatedString : public CompilerError {
   public:
    UnterminatedString() = default;
    [[nodiscard]] auto what() const noexcept -> const char * override;
    ~UnterminatedString() override = default;
};

/**
 * @brief A general compiler error, thrown at runtime either by the parser or interpreter
 */
class GeneralError : public std::runtime_error {
   private:
    Position pos;
    static auto format_message(const Position &pos, const std::string &msg) -> std::string;

   public:
    GeneralError(const Position &pos, const std::string &msg);
    ~GeneralError() override = default;
    [[nodiscard]] auto get_position() const -> const Position;
};

/**
 * @brief A parser's error thrown by the parser consisting of the error's position and message
 */
class ParserError : public GeneralError {
    public:
        ParserError(const Position& pos, const std::string &msg) : GeneralError(pos, msg) {}
};

/**
 * @brief A general interpreter error
 *
 * This error is supposed to be caught by a syntax tree element, that will include its position and throw a general error in its place
 */
class InterpreterError : public std::runtime_error {
    public:
        InterpreterError(const std::string &msg) : std::runtime_error(msg) {}
};
