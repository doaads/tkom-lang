#include "lexeme_context.h"
#include "tokens.h"
#include <cmath>


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
    unsigned int divisor = pow(10, decimal_places);
    lexeme = std::get<double>(lexeme) / ((divisor) ? divisor : 1);
    decimal_places = 0;
    return std::get<double>(lexeme);
}

void LexemeContext::add_char(char current_char) {
    if (!has_value() || !std::holds_alternative<std::string>(lexeme)) {
        lexeme = std::string(1, current_char);
    } else {
        std::get<std::string>(lexeme) += current_char;
    }
}

void LexemeContext::add_double(int digit) {
    if (!std::holds_alternative<double>(lexeme)) {
        lexeme = (double)std::get<int>(lexeme);
    }
    lexeme = std::get<double>(lexeme) * 10 + digit;
    decimal_places++;
}

void LexemeContext::add_int(int digit) {
    if (!has_value() || !std::holds_alternative<int>(lexeme)) {
        lexeme = digit;
    } else {
        lexeme = std::get<int>(lexeme) * 10 + digit;
    }
}

void LexemeContext::set_token_type(TokenType type) {
    current_token = type;
}

TokenType LexemeContext::get_token_type() {
    return current_token;
}

void LexemeContext::reset() {
    lexeme = std::monostate();
    decimal_places = 0;
}

bool LexemeContext::has_value() const {
    return lexeme.index() != 0;
}

std::variant<std::monostate, std::string, int, double> LexemeContext::get_lexeme() {
    return lexeme;
}
