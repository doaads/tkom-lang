#include "lexeme_context.h"
#include "tokens.h"


std::string LexemeContext::get_lexeme_string() {
    return std::get<std::string>(lexeme);
}

int LexemeContext::get_lexeme_int() {
    return std::get<int>(lexeme);
}

double LexemeContext::convert_to_double() {
    unsigned int divisor = 10.0 * decimal_places;
    lexeme = 1.0 * std::get<int>(lexeme) / ((divisor) ? divisor : 1.0);
    return std::get<double>(lexeme);
}

void LexemeContext::add_char(char current_char) {
    std::get<std::string>(lexeme) += current_char;
}

void LexemeContext::add_int(int digit) {
    if (std::holds_alternative<std::string>(lexeme)) {
        lexeme = digit;
    } else {
        lexeme = std::get<int>(lexeme) * 10 + digit;
    }
}

void LexemeContext::add_double(int digit) {
    add_int(digit);
    decimal_places++;
}

void LexemeContext::set_token_type(TokenType type) {
    current_token = type;
}

TokenType LexemeContext::get_token_type() {
    return current_token;
}
