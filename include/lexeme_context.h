#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include "tokens.h"


class LexemeContext {
    private:
        std::variant<std::monostate, std::string, int, double, bool> lexeme;
        uint8_t length = 0;
        TokenType current_token;
        bool blocked = false;
    public:
        template<typename T>
        T get_lexeme_as() const {
            return std::get<T>(lexeme);
        }

        std::variant<std::monostate, std::string, int, double, bool> get_lexeme();

        double convert_to_double();
        void convert_to_bool();

        void add_char(char current_char);
        void add_int(int digit);  // will convert std::variant to int
        void add_double(int digit);

        void set_token_type(TokenType type);
        TokenType get_token_type();

        void reset();
        bool has_value() const;

        void block_input();
        void unblock();
};
