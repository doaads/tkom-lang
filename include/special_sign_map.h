#pragma once

#include "tokens.h"
#include <map>

class SpecialSignMap {
    private:
        static const std::map<char, TokenType> sign_tokens;
    public:
        TokenType operator[](char current_char) const;
        bool contains(char current_char) const;
};
