#include "lexeme_context.h"
#include "tokens.h"
#include "exceptions.h"
#include <cmath>

#define MAX_IDENTIFIER_LENGTH 32


std::string LexemeContext::get_lexeme_string() {
    return std::get<std::string>(lexeme);
}

int LexemeContext::get_lexeme_int() {
    return std::get<int>(lexeme);
}

double LexemeContext::get_lexeme_double() {
    return std::get<double>(lexeme);
}

double LexemeContext::convert_to_double() {
    if (!std::holds_alternative<double>(lexeme)) {
        lexeme = (double)std::get<int>(lexeme);
    }
    unsigned int divisor = pow(10, length);
    lexeme = std::get<double>(lexeme) / ((divisor) ? divisor : 1);
    length = 0;
    return std::get<double>(lexeme);
}

void LexemeContext::convert_to_bool() {
    if (std::holds_alternative<std::string>(lexeme)) {
        std::string value = std::get<std::string>(lexeme);
        if (value == "true") {
            lexeme = true;
        } else if (value == "false") {
            lexeme = false;
        }
    }
}

void LexemeContext::add_char(char current_char) {
    if (blocked) {
        return;
    }
    if (!has_value() || !std::holds_alternative<std::string>(lexeme)) {
        lexeme = std::string(1, current_char);
        length = 1;
    } else {
        if (length >= MAX_IDENTIFIER_LENGTH) {
            blocked = true;
            throw IdentifierLengthExceeded();
        }
        std::get<std::string>(lexeme) += current_char;
        length++;
    }
}

void LexemeContext::add_double(int digit) {
    if (!std::holds_alternative<double>(lexeme)) {
        lexeme = (double)std::get<int>(lexeme);
    }
    if (blocked) {
        return;
    }
    lexeme = std::get<double>(lexeme) * 10 + digit;
    length++;
}

void LexemeContext::add_int(int digit) {
    if (blocked) {
        return;
    }
    if (!std::holds_alternative<int>(lexeme)) {
        lexeme = digit;
        return;
    }
    if (std::get<int>(lexeme) > (std::numeric_limits<int>::max() - digit) / 10) {
        blocked = true;
        throw OverflowWarning();
    }

    lexeme = std::get<int>(lexeme) * 10 + digit;
}

void LexemeContext::set_token_type(TokenType type) {
    current_token = type;
}

TokenType LexemeContext::get_token_type() {
    return current_token;
}

void LexemeContext::reset() {
    blocked = false;
    lexeme = std::monostate();
    length = 0;
}

bool LexemeContext::has_value() const {
    return lexeme.index() != 0;
}

std::variant<std::monostate, std::string, int, double, bool> LexemeContext::get_lexeme() {
    return lexeme;
}

void LexemeContext::block_input() {
    blocked = true;
}

void LexemeContext::unblock() {
    blocked = false;
}
