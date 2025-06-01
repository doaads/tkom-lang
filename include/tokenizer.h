#pragma once

#include <map>
#include <unordered_map>

#include "input_manager.h"
#include "token.h"

using MapTokenType = std::variant<std::monostate, TokenType>;
using OptToken = std::optional<Token>;

/**
 * @brief class representing the main part of the lexical analyzer
 */
class Tokenizer {
   private:
    static const std::unordered_map<std::string, TokenType> keyword_tokens;  //< map keyword to token types
    static const std::unordered_map<char, TokenType> first_char_tokens;  //< map first characters of operators
    static const std::unordered_map<char, TokenType> operator_tokens;  //< map short operators
    static const std::map<std::pair<char, TokenType>, TokenType> long_operator_tokens;  //< map long operators

    std::shared_ptr<InputManager> input;  //< the input manager, tracking the position in the source document

    /**
     * @brief start building a token at the given position
     *
     * Start building a token, skipping through all whitespace until a non-whitespace
     * character is found
     *
     * @param pos the current position in the source document
     */
    auto start_build_token(Position& pos) -> Token;

    /**
     * @brief attempt to create a token from the given character
     *
     * Try to build each token going through the possible ones one-by-one
     * Throw if none of the builders return a valid token
     *
     * @param current_char the character from which the token starts
     */
    auto build_token(char current_token) -> Token;

    /**
     * @brief try building an identifier token consisting of alphanumeric chars and '_'
     *
     * The character must be an alphanumeric char or '_'
     *
     * @param current_char the character from which the token starts
     */
    [[nodiscard]] auto build_identifier(char current_char) const -> OptToken;

    /**
     * @brief try building a number, integer or float
     *
     * The character must be a non-zero digit
     *
     * @param current_char the character from which the token starts
     */
    [[nodiscard]] auto build_number(char current_char) const -> OptToken;

    /**
     * @brief try building a string
     *
     * The character must be a '"'
     *
     * @param current_char the character from which the token starts
     */
    [[nodiscard]] auto build_string(char current_char) const -> OptToken;

    /**
     * @brief try building a short operator
     *
     * The character must be a valid one-char-long operator
     *
     * @param current_char the character from which the token starts
     */
    [[nodiscard]] auto build_short_operator(char current_char) const -> OptToken;

    /**
     * @brief try building a multi-character operator
     *
     * The character must be a valid first-character of a long operator
     *
     * @param current_char the character from which the token starts
     */
    [[nodiscard]] auto start_build_long_operator(char current_char) const -> OptToken;

    /**
     * @brief continue building a long operator
     * 
     * The character must be a valid continuation of the previously created operator
     *
     * @param type the type of the operator which we will extend
     */
    [[nodiscard]] auto build_long_operator(TokenType type) const -> OptToken;

    /**
     * @brief get the token type for a short operator represented by a given char
     */
    [[nodiscard]] auto get_type_for_operator(char c) const -> MapTokenType;

    /**
     * @brief get the token type for a start of a long operator represented by a given char
     */
    [[nodiscard]] auto get_type_for_first_op_char(char c) const -> MapTokenType;

    /**
     * @brief get the continuation of a currently built operator, for a given char
     *
     * @param type the type of the token we are extending
     */
    [[nodiscard]] auto get_type_for_long_op(char c, TokenType type) const -> MapTokenType;

    /**
     * @brief get the keyword token type for a keyword
     */
    [[nodiscard]] auto get_keyword_for_string(std::string value) const -> MapTokenType;

    /**
     * @brief get the char for an escape
     */
    [[nodiscard]] auto get_char_for_escape(char c) const -> char;

   public:
    Tokenizer(std::shared_ptr<InputManager> input);

    /**
     * @brief request the next token
     *
     * Continue parsing the file until a full token is built, then return the token
     */
    auto get_token() -> Token;
};
