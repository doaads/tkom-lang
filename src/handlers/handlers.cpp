#include "handlers.h"
#include "tokens.h"
#include "operator_map.h"
#include "special_sign_map.h"

const OperatorMap op_map;
const LongOperatorMap long_op_map;
const LongOperatorFirstCharMap op_first_char_map;
const SpecialSignMap special_sign_map;

// IN WHITESPACE HANDLER

DFAState InWhitespaceHandler::next_state(char current_char, LexemeContext& context) const {
    if (std::isspace(current_char)) {
        return DFAState::IN_WHITESPACE;

    } else if (std::isalpha(current_char)) {
        context.add_char(current_char);
        context.set_token_type(TokenType::T_IDENTIFIER);
        return DFAState::IN_IDENTIFIER;

    } else if (std::isdigit(current_char)){
        context.add_int(current_char - '0');
        context.set_token_type(TokenType::T_INT);
        return DFAState::IN_NUMBER;

    } else if (current_char == '"'){
        context.set_token_type(TokenType::T_STRING);
        return DFAState::IN_STRING;

    } else if (op_map.contains(current_char)){
        context.set_token_type(op_map[current_char]);
        return DFAState::IN_OPERATOR;

    } else if (op_first_char_map.contains(current_char)) {
        context.set_token_type(op_first_char_map[current_char]);
        return DFAState::IN_FIRST_CHAR_LONG_OP;
    } else if (special_sign_map.contains(current_char)) {
        context.set_token_type(special_sign_map[current_char]);
        return DFAState::END;
    }
    return DFAState::END;
}

// IN IDENTIFIER HANDLER

DFAState InIdentifierHandler::next_state(char current_char, LexemeContext& context) const {
    if (isalnum(current_char) || current_char == '_') {
        context.add_char(current_char);
        return DFAState::IN_IDENTIFIER;
    } else {
        return DFAState::END;
    }
}

// IN NUMBER HANDLER

DFAState InNumberHandler::next_state(char current_char, LexemeContext& context) const {
    if (isdigit(current_char)) {
        context.add_int(current_char - '0');
        return DFAState::IN_NUMBER;
    } else if (current_char == '.') {
        context.set_token_type(TokenType::T_FLT);
        return DFAState::IN_FLT;
    }
    return DFAState::END;
}

// IN FLT HANDLER


DFAState InFltHandler::next_state(char current_char, LexemeContext& context) const {
    if (isdigit(current_char)) {
        context.add_double(current_char - '0');
        return DFAState::IN_FLT;
    }

    context.convert_to_double();
    return DFAState::END;
}


// IN STRING HANDLER

DFAState InStringHandler::next_state(char current_char, LexemeContext& context) const {
    if (current_char == '"') {
        return DFAState::END;
    } else if (current_char == '\\') {
        return DFAState::IN_ESCAPE;
    }
    context.add_char(current_char);
    return DFAState::IN_STRING;
}

// IN ESCAPE HANDLER

DFAState InEscapeHandler::next_state(char current_char, LexemeContext& context) const {
    context.add_char(current_char);
    return DFAState::IN_STRING;
}

// IN OPERATOR HANDLER

DFAState InOperatorHandler::next_state(char current_char, LexemeContext& context) const {
    if (long_op_map.contains(std::pair<char, TokenType>(current_char, context.get_token_type()))) {
        context.set_token_type(long_op_map[std::pair<char, TokenType>(current_char, context.get_token_type())]);
        return DFAState::IN_LONG_OPERATOR;
    }

    return DFAState::END;
}

// IN LONG OPERATOR HANDLER

DFAState InLongOperatorHandler::next_state(char current_char, LexemeContext& context) const {
    if (long_op_map.contains(std::pair<char, TokenType>(current_char, context.get_token_type()))) {
        context.set_token_type(long_op_map[std::pair<char, TokenType>(current_char, context.get_token_type())]);
        return DFAState::IN_LONG_OPERATOR;
    }
    return DFAState::END;
}


// IN FIRST CHAR LONG OP HANDLER

DFAState InFirstCharLongOpHandler::next_state(char current_char, LexemeContext& context) const {
    if (long_op_map.contains(std::pair<char, TokenType>(current_char, context.get_token_type()))) {
        return DFAState::IN_LONG_OPERATOR;
    }
    context.set_token_type(TokenType::T_ERROR);
    return DFAState::END;
}
