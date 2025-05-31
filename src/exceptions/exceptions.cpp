#include "exceptions.h"

void CompilerException::set_position(Position pos) { position = pos; }

auto CompilerException::get_position() const -> const Position { return position; }

auto CompilerWarning::is_error() const -> bool { return false; }

auto CompilerError::is_error() const -> bool { return true; }

auto OverflowWarning::what() const noexcept -> const char * {
    return "\033[1;33mWARNING:\033[0m Integer Overflow";
}

auto UnexpectedToken::what() const noexcept -> const char * {
    return "\033[1;31mERROR:\033[0m Unexpected token";
}

auto IdentifierLengthExceeded::what() const noexcept -> const char * {
    return "\033[1;31mERROR:\033[0m Identifier Length Exceeded";
}

auto UnterminatedString::what() const noexcept -> const char * {
    return "\033[1;31mERROR:\033[0m Unterminated string";
}

/* ----------------------------- Parser -------------------------------- */

GeneralError::GeneralError(const Position &pos, const std::string &msg)
    : std::runtime_error(format_message(pos, msg)), pos(pos) {}

auto GeneralError::get_position() const -> const Position { return pos; }

auto GeneralError::format_message(const Position &pos, const std::string &msg) -> std::string {
    return "\033[1;31mException thrown at " + std::to_string(pos.get_line()) + ":" +
           std::to_string(pos.get_column()) + "\033[0m - " + msg;
}
