#include "tokenizer.h"

Tokenizer::Tokenizer(std::shared_ptr<InputStream> input) : input_stream(input) {}

Token Tokenizer::get_token() {
    context.reset();
    current_state = DFAState::IN_WHITESPACE;
    while (current_state != DFAState::END) {
        current_state = manager[current_state]->next_state(input_stream->get_next_char(), context);
    }

    Token result;

    if (context.get_token_type() == TokenType::T_IDENTIFIER) {
        // TODO: Map keyword to identifier
    } else {
        result.type = context.get_token_type();
    }

    if (context.has_value()) {
        // TODO: Set token value
    }
    return result;
}
