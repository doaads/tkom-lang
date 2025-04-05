#pragma once


#include <map>
#include "tokens.h"


class OperatorMap {
    private:
        static const std::map<char, TokenType> operator_tokens;
    public:
        TokenType operator[](char current_char) const;
        bool contains(char current_char) const;
};

class LongOperatorMap {
    private:
        static const std::map<std::pair<char, TokenType>, TokenType> long_operator_tokens;
    public:
        TokenType operator[](std::pair<char, TokenType> pair) const;
        bool contains(std::pair<char, TokenType>) const;
};

class LongOperatorFirstCharMap {
    private:
        static const std::map<char, TokenType> first_char_tokens;
    public:
        TokenType operator[](char current_char) const;
        bool contains(char current_char) const;
};
