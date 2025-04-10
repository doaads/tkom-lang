#pragma once

#include "position.h"
#include "tokens.h"
#include <string>
#include <variant>
#include <iostream>

using ValueType = std::variant<std::monostate, std::string, int, double, bool>;

class Token {
    private:
        TokenType type;
        ValueType value;
        Position position;
    public:
        Token() = default;
        Token(TokenType type);
        template<typename T>
        T get_value() const {
            return std::get<T>(value);
        }
        ValueType get_value() const;
        template<typename T>
        void set_value(T new_value) {
            value = new_value;
        }
        TokenType get_type() const;
        void set_type(TokenType new_type);
        Position get_position() const;
        void set_position(Position pos);

        friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

