#include "tokenizer.h"

#include "keyword_map.h"
#include <iostream>
#include <unistd.h>

static const KeywordMap keyword_map;

Tokenizer::Tokenizer(std::shared_ptr<InputManager> input) : input(input) {}


Token Tokenizer::get_token() {
    context.reset();

    Token result;
    result.position = input->save_position();

    current_state = DFAState::IN_WHITESPACE;
    while (current_state != DFAState::END && !input->end()) {
        char next_char = input->get_next_char();
        current_state = manager[current_state]->next_state(next_char, context);
    }
    std::cout << "TOKEN PROCESSED!" << std::endl;
    input->unget();
    TokenType type = context.get_token_type();

    if (type == TokenType::T_COMMENT) {
        input->skip_line();
    }

    if (type == TokenType::T_IDENTIFIER && keyword_map.contains(context.get_lexeme_string())) {
        result.type = keyword_map[context.get_lexeme_string()];
    } else {
        result.type = context.get_token_type();
    }

    if (context.has_value()) {
        result.value = context.get_lexeme();
    }
    return result;
}
