#include "exceptions.h"

//OverflowWarning::OverflowWarning() = default;
//OverflowWarning::~OverflowWarning() = default;

const char* OverflowWarning::what() const noexcept {
    return "Integer Overflow - MAX_INT value exceeded";
}

const char* UnexpectedToken::what() const noexcept {
    return "Unexpected Token";
}

const char* IdentifierLengthExceeded::what() const noexcept {
    return "Identifier length exceeded - Only the first 32 characters will be processed";
}
