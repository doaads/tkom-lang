#include "tokenizer.h"

#include "keyword_map.h"
#include <unistd.h>
#include "exceptions.h"

static const KeywordMap keyword_map;

Tokenizer::Tokenizer(std::shared_ptr<InputManager> input) : input(input) {}


Token Tokenizer::get_token() {
    context.reset();

    Token result;
    bool saved_position = false;

    current_state = DFAState::IN_WHITESPACE;
    while (current_state != DFAState::END && !input->end()) {
        char next_char = input->get_next_char();
        try {
            current_state = manager[current_state]->next_state(next_char, context);
        } catch (CompilerException& e) {
            Position pos = input->save_position();
            exception_handler.print_exception_message(e, pos);
            if (exception_handler.is_error(e)) {
                result.type = TokenType::T_ERROR;
                result.position = pos;
                return result;
            }
        }
        if (current_state != DFAState::IN_WHITESPACE && !saved_position) {
            result.position = input->save_position();
            saved_position = true;
        }
    }

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
