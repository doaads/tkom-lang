#include "keyword_map.h"

const std::unordered_map<std::string, TokenType> KeywordMap::keyword_tokens {
    {"int", TokenType::T_INT_TYPE},
    {"flt", TokenType::T_FLT_TYPE},
    {"string", TokenType::T_STRING_TYPE},
    {"int", TokenType::T_INT_TYPE},
    {"void", TokenType::T_VOID_TYPE},
    {"ret", TokenType::T_RET},
    {"while", TokenType::T_WHILE},
    {"for", TokenType::T_FOR},
    {"if", TokenType::T_IF},
    {"elif", TokenType::T_ELIF},
    {"else", TokenType::T_ELSE},
    {"mut", TokenType::T_MUT},
    {"bool", TokenType::T_BOOL_TYPE},
    {"true", TokenType::T_BOOL},
    {"false", TokenType::T_BOOL},
};

TokenType KeywordMap::operator[](std::string value) const {
    auto it = keyword_tokens.find(value);
    if (it == keyword_tokens.end()) {
        return TokenType::T_ERROR;
    }
    return it->second;
}

bool KeywordMap::contains(std::string value) const {
    return keyword_tokens.count(value);
}
