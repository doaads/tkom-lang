#pragma once

#include "dfa_states.h"
#include "handler_manager.h"
#include "token.h"
#include "lexeme_context.h"
#include "input_manager.h"


class Tokenizer {
    private:
        LexemeContext context;
        std::shared_ptr<InputManager> input;
        DFAState current_state;
        HandlerManager manager;
    public:
        Tokenizer(std::shared_ptr<InputManager> input);
        Token get_token();
};
