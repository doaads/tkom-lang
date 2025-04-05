#pragma once

#include "tokens.h"
#include <string>
#include <unordered_map>

class KeywordMap {
    private:
        static const std::unordered_map<std::string, TokenType> keyword_tokens;
    public:
        TokenType operator[](std::string value) const;
        bool contains(std::string value) const;
};
