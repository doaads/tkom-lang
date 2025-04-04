#include "handlers.h"
#include "tokens.h"

// process - returns a token (if state is END)
// next_state - get next state from current char

void StateHandler::add_string(char current_char, std::variant<std::string, int, double>& lexeme) const {
    if (std::holds_alternative<std::string>(lexeme)) {
        std::get<std::string>(lexeme) += current_char;
    }
}

void StateHandler::add_int(char current_char, std::variant<std::string, int, double>& lexeme) const {
    if (std::holds_alternative<std::string>(lexeme)) {
        lexeme = current_char - '0';
    } else {
        lexeme = std::get<int>(lexeme) * 10 + (current_char - '0');
    }
}

// IN WHITESPACE HANDLER

TokenType InWhitespaceHandler::process(char current_char) {
    return TokenType::T_ERROR;
}

DFAState InWhitespaceHandler::next_state(char current_char, std::variant<std::string, int, double>& lexeme) const {
    if (std::isspace(current_char)) {
        add_string(current_char, lexeme);
        return DFAState::IN_WHITESPACE;
    } else if (std::isalpha(current_char)) {
        add_string(current_char, lexeme);
        return DFAState::IN_IDENTIFIER;
    } else if (std::isdigit(current_char)){
        add_int(current_char, lexeme);
        return DFAState::IN_NUMBER;
    } else {
        return DFAState::END;
    }
}

// IN IDENTIFIER HANDLER

TokenType InIdentifierHandler::process(char current_char) {
    return TokenType::T_IDENTIFIER;
}

DFAState InIdentifierHandler::next_state(char current_char, std::variant<std::string, int, double>& lexeme) const {
    if (isalnum(current_char) || current_char == '_') {
        add_string(current_char, lexeme);
        return DFAState::IN_IDENTIFIER;
    } else {
        return DFAState::END;
    }
}

// IN NUMBER HANDLER

TokenType InNumberHandler::process(char current_char) {
    return TokenType::T_INT;
}

DFAState InNumberHandler::next_state(char current_char, std::variant<std::string, int, double>& lexeme) const {
    if (isdigit(current_char)) {
        add_int(current_char, lexeme);
        return DFAState::IN_NUMBER;
    } else if (current_char == '.') {
        return DFAState::IN_FLT;
    }
    return DFAState::END;
}
