#include "exceptions.h"

void CompilerException::set_position(Position pos) {
    position = pos;
}

const Position CompilerException::get_position() const {
    return position;
}

bool CompilerWarning::is_error() const{
    return false;
}

bool CompilerError::is_error() const{
    return true;
}


const char* OverflowWarning::what() const noexcept {
    return "\033[1;33mWARNING:\033[0m Integer Overflow";
}

const char* UnexpectedToken::what() const noexcept {
    return "\033[1;31mERROR:\033[0m Unexpected token";
}

const char* IdentifierLengthExceeded::what() const noexcept {
    return "\033[1;31mERROR:\033[0m Identifier Length Exceeded";
}

const char* UnterminatedString::what() const noexcept {
    return "\033[1;31mERROR:\033[0m Unterminated string";
}
