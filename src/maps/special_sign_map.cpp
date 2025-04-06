#include "special_sign_map.h"

#include "tokens.h"
#include <map>

const std::map<char, TokenType> SpecialSignMap::sign_tokens {
    {'(', TokenType::T_LPAREN},
    {')', TokenType::T_RPAREN},
    {'{', TokenType::T_LBLOCK},
    {'}', TokenType::T_RBLOCK},
    {';', TokenType::T_SEMICOLON},
    {',', TokenType::T_COMMA},
    {'_', TokenType::T_WILDCARD},
};

TokenType SpecialSignMap::operator[](char current_char) const {
    auto it = sign_tokens.find(current_char);
    if (it == sign_tokens.end()) {
        return TokenType::T_ERROR;
    }
    return it->second;
}

bool SpecialSignMap::contains(char current_char) const {
    return sign_tokens.count(current_char);
}
