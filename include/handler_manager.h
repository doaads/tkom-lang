#pragma once

#include <map>
#include <memory>
#include "handlers.h"
#include "dfa_states.h"
#include "tokens.h"


class HandlerManager {
    private:
        std::map<DFAState, std::unique_ptr<StateHandler>> state_handlers;
        std::map<char, TokenType> operator_tokens;
        std::map<std::pair<char, TokenType>, TokenType> long_operator_tokens;
    public:
        HandlerManager();
        std::unique_ptr<StateHandler>& operator[](DFAState state);
};
