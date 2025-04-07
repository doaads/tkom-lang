#pragma once

#include "dfa_states.h"
#include "lexeme_context.h"


class StateHandler {
    public:
        virtual DFAState next_state(char current_char, LexemeContext& context) const = 0;
};

class InWhitespaceHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InIdentifierHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InNumberHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InFltHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InStringHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InEscapeHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InOperatorHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};


class InLongOperatorHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};

class InFirstCharLongOpHandler : public StateHandler {
    DFAState next_state(char current_char, LexemeContext& context) const override;
};
