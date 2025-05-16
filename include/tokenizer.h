#pragma once

#include <map>
#include <unordered_map>

#include "input_manager.h"
#include "token.h"

using MapTokenType = std::variant<std::monostate, TokenType>;

class Tokenizer {
   private:
    static const std::unordered_map<std::string, TokenType> keyword_tokens;
    static const std::unordered_map<char, TokenType> first_char_tokens;
    static const std::unordered_map<char, TokenType> operator_tokens;
    static const std::map<std::pair<char, TokenType>, TokenType> long_operator_tokens;

    std::shared_ptr<InputManager> input;

    Token build_token(Position &pos);

    Token build_identifier(char current_char) const;
    Token build_number(char current_char) const;
    Token build_string(char current_char) const;
    Token build_short_operator(char current_char) const;
    Token start_build_long_operator(char current_char) const;

    Token build_long_operator(TokenType type) const;

    MapTokenType get_type_for_operator(char c) const;
    MapTokenType get_type_for_first_op_char(char c) const;
    MapTokenType get_type_for_long_op(char c, TokenType type) const;
    MapTokenType get_keyword_for_string(std::string value) const;
    char get_char_for_escape(char c) const;

   public:
    Tokenizer(std::shared_ptr<InputManager> input);
    Token get_token();
};
