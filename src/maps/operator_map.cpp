#include "operator_map.h"

const std::map<char, TokenType> OperatorMap::operator_tokens {
    {'-', TokenType::T_MINUS},
    {'+', TokenType::T_PLUS},
    {'*', TokenType::T_MULT},
    {'/', TokenType::T_DIV},
    {'@', TokenType::T_DECORATE},
    {'-', TokenType::T_MINUS},
    {'!', TokenType::T_NOT},
    {'>', TokenType::T_GT},
    {'<', TokenType::T_LT},
};

TokenType OperatorMap::operator[](char current_char) const {
    auto it = operator_tokens.find(current_char);
    if (it == operator_tokens.end()) {
        return TokenType::T_ERROR;
    }
    return it->second;
}

bool OperatorMap::contains(char current_char) const {
    return operator_tokens.count(current_char);
}

const std::map<std::pair<char, TokenType>, TokenType> LongOperatorMap::long_operator_tokens {
    {std::pair<char, TokenType>('>', TokenType::T_MINUS), TokenType::T_CALL},
    {std::pair<char, TokenType>('>', TokenType::T_CALL), TokenType::T_BINDFRT},
    {std::pair<char, TokenType>('=', TokenType::T_NOT), TokenType::T_NEQ},
    {std::pair<char, TokenType>('=', TokenType::T_GT), TokenType::T_GTE},
    {std::pair<char, TokenType>('=', TokenType::T_LT), TokenType::T_LTE},
    {std::pair<char, TokenType>('>', TokenType::T_EQ), TokenType::T_ASSIGN},
    {std::pair<char, TokenType>('=', TokenType::T_EQ), TokenType::T_EQ},
    {std::pair<char, TokenType>('&', TokenType::T_AND), TokenType::T_AND},
    {std::pair<char, TokenType>('|', TokenType::T_OR), TokenType::T_OR},
    {std::pair<char, TokenType>('/', TokenType::T_DIV), TokenType::T_COMMENT},
};

TokenType LongOperatorMap::operator[](std::pair<char, TokenType> pair) const {
    auto it = long_operator_tokens.find(pair);
    if (it == long_operator_tokens.end()) {
        return TokenType::T_ERROR;
    }
    return it->second;
}

bool LongOperatorMap::contains(std::pair<char, TokenType> pair) const {
    return long_operator_tokens.count(pair);
}

const std::map<char, TokenType> LongOperatorFirstCharMap::first_char_tokens {
    {'=', TokenType::T_EQ},
    {'|', TokenType::T_OR},
    {'&', TokenType::T_AND},
};

TokenType LongOperatorFirstCharMap::operator[](char current_char) const {
    auto it = first_char_tokens.find(current_char);
    if (it == first_char_tokens.end()) {
        return TokenType::T_ERROR;
    }
    return it->second;
}

bool LongOperatorFirstCharMap::contains(char current_char) const {
    return first_char_tokens.count(current_char);
}
