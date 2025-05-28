#pragma once

#include <map>
#include <unordered_map>

#include "input_manager.h"
#include "token.h"

using MapTokenType = std::variant<std::monostate, TokenType>;
using OptToken = std::optional<Token>;

class Tokenizer {
   private:
    static const std::unordered_map<std::string, TokenType> keyword_tokens;
    static const std::unordered_map<char, TokenType> first_char_tokens;
    static const std::unordered_map<char, TokenType> operator_tokens;
    static const std::map<std::pair<char, TokenType>, TokenType> long_operator_tokens;

    std::shared_ptr<InputManager> input;

    Token start_build_token(Position& pos);
    Token build_token(char current_token);

    OptToken build_identifier(char current_char) const;
    OptToken build_number(char current_char) const;
    OptToken build_string(char current_char) const;
    OptToken build_short_operator(char current_char) const;
    OptToken start_build_long_operator(char current_char) const;

    OptToken build_long_operator(TokenType type) const;

    MapTokenType get_type_for_operator(char c) const;
    MapTokenType get_type_for_first_op_char(char c) const;
    MapTokenType get_type_for_long_op(char c, TokenType type) const;
    MapTokenType get_keyword_for_string(std::string value) const;
    char get_char_for_escape(char c) const;

   public:
    Tokenizer(std::shared_ptr<InputManager> input);
    Token get_token();
};
