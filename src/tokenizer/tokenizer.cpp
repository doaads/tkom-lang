#include "tokenizer.h"

#include <cmath>
#include <utility>

#include "exceptions.h"

Tokenizer::Tokenizer(std::shared_ptr<InputManager> input) : input(std::move(input)) {}

auto Tokenizer::get_token() -> Token {
    Position pos = input->save_position();
    Token token = start_build_token(pos);
    token.set_position(pos);
    return token;
}

auto Tokenizer::start_build_token(Position& pos) -> Token {
    char current_char = input->get_next_char();
    while (current_char != EOF) {
        if (!std::isspace(current_char)) {
            return build_token(current_char);
        } else {
            pos = input->save_position();
            current_char = input->get_next_char();
        }
    }
    return {TokenType::T_EOF};
}

auto Tokenizer::build_token(char current_char) -> Token {
    OptToken token = build_identifier(current_char);
    if (token) return *token;

    token = build_number(current_char);
    if (token) return *token;

    token = build_string(current_char);
    if (token) return *token;

    token = build_short_operator(current_char);
    if (token) return *token;

    token = start_build_long_operator(current_char);
    if (token) return *token;

    throw UnexpectedToken();
}

auto Tokenizer::build_identifier(char current_char) const -> OptToken {
    if (!std::isalpha(current_char)) return std::nullopt;

    std::string identifier = "";
    unsigned short length = 0;
    while (isalnum(current_char) || current_char == '_') {
        identifier += current_char;
        length++;
        if (length >= 32) throw IdentifierLengthExceeded();
        current_char = input->get_next_char();
    }
    MapTokenType keyword = get_keyword_for_string(identifier);
    Token result;
    if (keyword.index()) {
        result.set_type(std::get<TokenType>(keyword));
        if (result.get_type() == TokenType::T_BOOL) {
            result.set_value<bool>(identifier == "true");
        }
    } else {
        result.set_type(TokenType::T_IDENTIFIER);
        result.set_value<std::string>(identifier);
    }
    input->unget();
    return result;
}

auto Tokenizer::build_number(char current_char) const -> OptToken {
    if (!std::isdigit(current_char)) return std::nullopt;

    unsigned long number = 0;
    bool floating = false;
    unsigned short decimal_places = 0;

    while (isdigit(current_char)) {
        unsigned short digit = current_char - '0';
        if (floating) {
            decimal_places++;
        } else if ((long)number > std::numeric_limits<int>::max() - digit / 10) {
            throw OverflowWarning();
        }
        number *= 10;
        number += digit;
        current_char = input->get_next_char();
        if (current_char == '.') {
            floating = true;
            current_char = input->get_next_char();
        }
    }
    input->unget();
    Token result = Token();

    if (floating) {
        result.set_type(TokenType::T_FLT);
        unsigned int divisor = pow(10, decimal_places);
        result.set_value<double>((double)number / ((divisor) ? divisor : 1));
    } else {
        result.set_type(TokenType::T_INT);
        result.set_value<int>(number);
    }
    return result;
}

auto Tokenizer::build_string(char current_char) const -> OptToken {
    if (current_char != '"') return std::nullopt;

    std::string value = "";
    current_char = input->get_next_char();
    while (current_char != '"') {
        if (current_char == '\\') {
            current_char = get_char_for_escape(input->get_next_char());
        } else if (current_char == '\n') {
            throw UnterminatedString();
        }
        value += current_char;
        current_char = input->get_next_char();
    }
    Token result = Token(TokenType::T_STRING);
    result.set_value<std::string>(value);
    return result;
}

auto Tokenizer::build_short_operator(char current_char) const -> OptToken {
    if (!operator_tokens.count(current_char)) return std::nullopt;

    TokenType type = std::get<TokenType>(get_type_for_operator(current_char));
    return build_long_operator(type);
}

auto Tokenizer::start_build_long_operator(char current_char) const -> OptToken {
    if (!first_char_tokens.count(current_char)) return std::nullopt;

    TokenType type = std::get<TokenType>(get_type_for_first_op_char(current_char));
    current_char = input->get_next_char();
    MapTokenType new_type = get_type_for_long_op(current_char, type);
    if (new_type.index()) {
        return build_long_operator(std::get<TokenType>(new_type));
    } else {
        throw UnexpectedToken();
    }
}

auto Tokenizer::build_long_operator(TokenType type) const -> OptToken {
    MapTokenType new_type = type;
    char current_char;
    do {
        current_char = input->get_next_char();
        type = std::get<TokenType>(new_type);
        new_type = get_type_for_long_op(current_char, type);
    } while (new_type.index());
    if (type == TokenType::T_COMMENT) input->skip_line();
    input->unget();
    return Token(type);
}

auto Tokenizer::get_type_for_operator(char c) const -> MapTokenType {
    auto it = operator_tokens.find(c);
    if (it != operator_tokens.end()) {
        return it->second;
    } else {
        return {std::monostate()};
    }
}

auto Tokenizer::get_type_for_first_op_char(char c) const -> MapTokenType {
    auto it = first_char_tokens.find(c);
    if (it != first_char_tokens.end()) {
        return it->second;
    } else {
        return {std::monostate()};
    }
}

auto Tokenizer::get_type_for_long_op(char c, TokenType type) const -> MapTokenType {
    std::pair<char, TokenType> pair = std::pair<char, TokenType>(c, type);
    auto it = long_operator_tokens.find(pair);
    if (it == long_operator_tokens.end()) {
        return {std::monostate()};
    }
    return it->second;
}

auto Tokenizer::get_keyword_for_string(std::string value) const -> MapTokenType {
    auto it = keyword_tokens.find(value);
    if (it == keyword_tokens.end()) {
        return {std::monostate()};
    }
    return it->second;
}

auto Tokenizer::get_char_for_escape(char c) const -> char {
    switch (c) {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        default:
            return c;
    }
}

const std::unordered_map<std::string, TokenType> Tokenizer::keyword_tokens{
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

const std::unordered_map<char, TokenType> Tokenizer::operator_tokens{
    {'-', TokenType::T_MINUS},    {'+', TokenType::T_PLUS},      {'*', TokenType::T_MULT},
    {'/', TokenType::T_DIV},      {'@', TokenType::T_DECORATE},  {'-', TokenType::T_MINUS},
    {'!', TokenType::T_NOT},      {'>', TokenType::T_GT},        {'<', TokenType::T_LT},
    {'(', TokenType::T_LPAREN},   {')', TokenType::T_RPAREN},    {'{', TokenType::T_LBLOCK},
    {'}', TokenType::T_RBLOCK},   {';', TokenType::T_SEMICOLON}, {',', TokenType::T_COMMA},
    {'_', TokenType::T_WILDCARD}, {'[', TokenType::T_LFTYPE},    {']', TokenType::T_RFTYPE}};

const std::map<std::pair<char, TokenType>, TokenType> Tokenizer::long_operator_tokens{
    {std::pair<char, TokenType>('>', TokenType::T_MINUS), TokenType::T_CALL},
    {std::pair<char, TokenType>('>', TokenType::T_CALL), TokenType::T_BINDFRT},
    {std::pair<char, TokenType>('=', TokenType::T_NOT), TokenType::T_NEQ},
    {std::pair<char, TokenType>('=', TokenType::T_GT), TokenType::T_GTE},
    {std::pair<char, TokenType>('=', TokenType::T_LT), TokenType::T_LTE},
    {std::pair<char, TokenType>('>', TokenType::T_EQ_ST), TokenType::T_ASSIGN},
    {std::pair<char, TokenType>('=', TokenType::T_EQ_ST), TokenType::T_EQ},
    {std::pair<char, TokenType>('&', TokenType::T_AND_ST), TokenType::T_AND},
    {std::pair<char, TokenType>('|', TokenType::T_OR_ST), TokenType::T_OR},
    {std::pair<char, TokenType>('/', TokenType::T_DIV), TokenType::T_COMMENT},
    {std::pair<char, TokenType>(':', TokenType::T_FUNC_SIGN_ST), TokenType::T_FUNC_SIGN},
};

const std::unordered_map<char, TokenType> Tokenizer::first_char_tokens{
    {'=', TokenType::T_EQ_ST},
    {'|', TokenType::T_OR_ST},
    {'&', TokenType::T_AND_ST},
    {':', TokenType::T_FUNC_SIGN_ST},
};
