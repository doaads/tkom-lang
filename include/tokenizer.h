#pragma once

#include "dfa_states.h"
#include "handler_manager.h"
#include "token.h"
#include "input_stream.h"
#include "lexeme_context.h"


class Tokenizer {
    private:
        LexemeContext context;
        std::shared_ptr<InputStream> input_stream;
        DFAState current_state;
        HandlerManager manager;
    public:
        Tokenizer(std::shared_ptr<InputStream> input);
        Token get_token();
};
