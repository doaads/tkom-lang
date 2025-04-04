#pragma once

#include "dfa_states.h"
#include "tokens.h"
#include <string>
#include <variant>


class StateHandler {
    public:
        virtual TokenType process(char current_char) = 0;
        virtual DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const = 0;
    protected:
        void add_string(char current_char, std::variant<std::string, int, double>& lexeme) const;
        void add_int(char current_char, std::variant<std::string, int, double>& lexeme) const;
        void add_double(char current_char);
};


class InWhitespaceHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InIdentifierHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InNumberHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InFltHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InStringHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InLtHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InGtHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InNotHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InEqHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InCallHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InMinusHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};


class InDivHandler : public StateHandler {
    TokenType process(char current_char) override;
    DFAState next_state(char current_char, std::variant<std::string, int, double>& lexeme) const override;
};
